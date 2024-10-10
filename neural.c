#include "neural.h"
#include "math.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

NeuralNetwork *neural_new(int no_layers, int *nodes_per_layer) {
  NeuralNetwork *nn = malloc(sizeof(NeuralNetwork));
  nn->no_layers = no_layers;
  nn->inputSize = nodes_per_layer[0];
  nn->outputSize = nodes_per_layer[no_layers - 1];
  nn->layers = malloc(sizeof(Layer) * no_layers);

  // we subtract one from the number of nodes since the output layer doesn't
  // have weights
  for (int i = 0; i < no_layers - 1; i++) {
    nn->layers[i].from = nodes_per_layer[i];
    nn->layers[i].to = nodes_per_layer[i + 1];
    nn->layers[i].weights =
        // we add +1 in the row count since this row represents the bias
        matrix_new(nodes_per_layer[i] + 1, nodes_per_layer[i + 1], 0.01);
  }

  nn->activations = malloc(sizeof(Matrix *) * no_layers - 1);
  // allocate activations, start at 1 because 0 is the input layer
  for (int i = 1; i < no_layers - 1; i++) {
    nn->activations[i] = matrix_new(1, nodes_per_layer[i], 0);
  }

  return nn;
}

// apply sigmoid activation function on the matrix
Matrix *apply_activation(Matrix *input) {
  Matrix *result = matrix_new(input->rows, input->cols, 0);
  for (int i = 0; i < input->rows; i++) {
    for (int j = 0; j < input->cols; j++) {
      double val = matrix_get(input, i, j);
      val = 1 / (1 + exp(-val));
      matrix_set(result, i, j, val);
    }
  }

  return result;
}

Matrix *neural_run(NeuralNetwork *nn, Matrix *input) {
  Matrix *currentMatrix = input;
  nn->activations[0] = currentMatrix;

  for (int i = 0; i < nn->no_layers - 1; i++) {
    Matrix *result = matrix_mult(currentMatrix, nn->layers[i].weights);
    Matrix *activation = apply_activation(result);
    nn->activations[i + 1] = activation;

    currentMatrix = activation;
    matrix_free(result);
  }

  return currentMatrix;
}

Matrix *neural_compute_layer_loss(Matrix *expected, Matrix *actual) {
  Matrix *losses = matrix_new(expected->rows, expected->cols, 0);
  for (int i = 0; i < expected->rows; i++) {
    for (int j = 0; j < expected->cols; j++) {
      double loss = (matrix_get(expected, i, j) - (matrix_get(actual, i, j)));
      matrix_set(losses, i, j, loss);
    }
  }

  return losses;
}

// Update the weights of a single layer
void neural_adjust_layer_n(NeuralNetwork *nn, Matrix *losses, int layer_n) {
  if (layer_n > nn->no_layers - 2) {
    printf("Error: layer_n is out of bounds\n");
    return;
  }

  Layer *layer = &(nn->layers[layer_n]);

  for (int j = 0; j < layer->weights->cols; j++) {

    // iterate through each of the rows in the column, each item we'll iterate
    // through corresponds to an x_i
    for (int i = 0; i < layer->weights->rows; i++) {
      double loss = matrix_get(losses, 0, j);

      // get the input from this weight
      double x_i =
          i == layer->weights->rows - 1
              ? 1 // last row is the bias so the weight is automatically 1
              : matrix_get(nn->activations[layer_n], 0, i);

      double gradient = LEARNING_RATE * loss * x_i;
      double weight = matrix_get(layer->weights, i, j);

      // printf("Updating weight %d %d to %.2f from loss: %.2f and x_i: %.2f\n",
      // i,
      //  j, weight, loss, x_i);
      matrix_set(layer->weights, i, j, weight + gradient);
    }
  }

  matrix_print(layer->weights, 0);
}