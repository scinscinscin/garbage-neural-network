#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

Matrix *matrix_new(int rows, int cols, double _default) {
  Matrix *m = malloc(sizeof(Matrix));
  m->rows = rows;
  m->cols = cols;
  m->data = calloc(rows * cols, sizeof(double));
  for (int i = 0; i < rows * cols; i++)
    m->data[i] = _default;

  return m;
}

void matrix_free(Matrix *m) {
  free(m->data);
  free(m);
}

void matrix_set(Matrix *m, int row, int col, double val) {
  m->data[row * m->cols + col] = val;
}

double matrix_get(Matrix *m, int row, int col) {
  return m->data[row * m->cols + col];
}

void matrix_print(Matrix *m, int cols) {
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      printf("%.2f ", m->data[i * m->cols + j]);
      if (cols != 0 && j % cols == cols - 1)
        printf("\n");
    }
    printf("\n");
  }
}

Matrix *matrix_mult(Matrix *first, Matrix *second) {
  if (first->cols != second->rows - 1) { // last row on the 2nd matrix is bias
    printf("Error: matrix dimensions don't match\n");
    return NULL;
  }

  Matrix *result = matrix_new(first->rows, second->cols, 0);
  for (int i = 0; i < first->rows; i++) {
    for (int j = 0; j < second->cols; j++) {

      double sum = 0;
      // loop through each of the columns in the first matrix and multiply with
      // their corresponding row in the second matrix
      for (int k = 0; k < first->cols; k++) {
        sum += matrix_get(first, i, k) * matrix_get(second, k, j);
      }

      // add the bias
      sum += matrix_get(second, first->cols, j);

      matrix_set(result, i, j, sum);
    }
  }
  return result;
}

Matrix *matrix_sub(Matrix *first, Matrix *second) {
  if (first->rows != second->rows || first->cols != second->cols) {
    printf("Error: matrix dimensions don't match\n");
    return NULL;
  }

  Matrix *result = matrix_new(first->rows, first->cols, 0);
  for (int i = 0; i < first->rows; i++) {
    for (int j = 0; j < first->cols; j++) {
      matrix_set(result, i, j,
                 matrix_get(first, i, j) - matrix_get(second, i, j));
    }
  }

  return result;
}

Matrix *matrix_collapse_side(Matrix *m) {
  Matrix *ret = matrix_new(m->rows, 1, 0);

  for (int i = 0; i < m->rows; i++) {
    double sum = 0;
    for (int j = 0; j < m->cols; j++) {
      sum += matrix_get(m, i, j);
    }

    matrix_set(ret, i, 0, sum);
  }

  return ret;
}

Matrix *matrix_transpose(Matrix *m) {
  Matrix *result = matrix_new(m->cols, m->rows, 0);
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      matrix_set(result, j, i, matrix_get(m, i, j));
    }
  }
  return result;
}