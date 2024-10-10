#include "matrix.h"
#define LEARNING_RATE 0.01

typedef struct {
  int from;
  int to;
  Matrix *weights;
} Layer;

typedef struct {
  int no_layers;
  int inputSize;  // column vector with 384 elements
  int outputSize; // column vector with 10 elements
  Matrix **activations;
  Layer *layers;
} NeuralNetwork;

NeuralNetwork *neural_new(int no_layers, int *nodes_per_layer);
Matrix *neural_run(NeuralNetwork *nn, Matrix *input);
Matrix *neural_compute_layer_loss(Matrix *expected, Matrix *actual);
void neural_adjust_layer_n(NeuralNetwork *nn, Matrix *losses, int layer_n);