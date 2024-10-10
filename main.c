#include "matrix.h"
#include "neural.h"
#include "utils.h"
#include <stdio.h>

Matrix *get_label_matrix(int label) {
  Matrix *m = matrix_new(1, 10, 0);
  matrix_set(m, 0, label, 1);
  return m;
}

void train_sample(NeuralNetwork *nn, Sample *sample) {
  Matrix *result = neural_run(nn, sample->input);
  Matrix *expected = get_label_matrix(sample->label);

  // Begin back propogation right here
  Matrix *losses =
      neural_compute_layer_loss(expected, result); // loss of the output layer
  neural_adjust_layer_n(nn, losses, 1); // adjust the weights of the 2nd layer

  // what the middle layer wants to be
  Matrix *collapsed = matrix_collapse_side(nn->layers[1].weights);
  Matrix *transposed = matrix_transpose(collapsed);

  Matrix *losses2 = neural_compute_layer_loss(
      transposed, nn->activations[1]);   // compute the loss of the 1st layer
  neural_adjust_layer_n(nn, losses2, 0); // adjust the weights of the 1st layer
  // matrix_print(losses2, 0);

  // let go and be free
  matrix_free(losses);
  matrix_free(losses2);
  matrix_free(collapsed);
  matrix_free(result);
  matrix_free(expected);
  matrix_free(transposed);
}

#define MIN(A, B) ((A) > (B) ? (B) : (A))

void perform_testing(NeuralNetwork *nn, int max) {
  // the network has been trained, now let's test it
  int test_sample_count = 0;
  Sample *test_samples =
      load_samples("MNIST/mnist_test.csv", &test_sample_count);

  for (int i = 0; i < MIN(test_sample_count, max); i++) {
    Sample *sample = &test_samples[i];
    Matrix *result = neural_run(nn, sample->input);

    double current_confidence = 0;
    int answer = 0;
    for (int j = 0; j < 10; j++) {
      double val = matrix_get(result, 0, j);
      if (val > current_confidence) {
        current_confidence = val;
        answer = j;
      }
    }

    printf("Answer: %d, confidence: %.2f, real: %d\n", answer,
           current_confidence, sample->label);
  }
}

#define MAX_SAMPLE 1
int main(int argc, char *argv[]) {
  int sample_count = 0;
  Sample *samples = load_samples("MNIST/mnist_train.csv", &sample_count);

  NeuralNetwork *nn = neural_new(3, (int[]){784, 100, 10});
  for (int i = 0; i < MIN(MAX_SAMPLE, sample_count); i++) {
    printf("Training sample %d, label: %d\n", i, samples[i].label);
    train_sample(nn, &samples[i]);
  }

  // matrix_print(nn->layers[1].weights, 0);
  // perform_testing(nn, 100);
  // return 0;
}
