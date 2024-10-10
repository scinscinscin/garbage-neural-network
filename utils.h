#include "matrix.h"
typedef struct {
  int label;
  Matrix *input;
} Sample;

Sample *load_samples(const char *location, int *sample_count);