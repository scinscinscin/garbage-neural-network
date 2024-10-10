#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  int rows;
  int cols;
  double *data;
} Matrix;

Matrix *matrix_new(int rows, int cols, double _default);
void matrix_free(Matrix *m);
void matrix_set(Matrix *m, int row, int col, double val);
double matrix_get(Matrix *m, int row, int col);
void matrix_print(Matrix *m, int rows);
Matrix *matrix_mult(Matrix *first, Matrix *second);
Matrix *matrix_sub(Matrix *first, Matrix *second);
Matrix *matrix_collapse_side(Matrix *m);
Matrix *matrix_transpose(Matrix *m);
#endif