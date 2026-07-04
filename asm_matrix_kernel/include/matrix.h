#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

double dot_f64_c(const double *a, const double *b, size_t n);
double dot_f64_asm(const double *a, const double *b, size_t n);
void vec_add_f64_c(const double *a, const double *b, double *out, size_t n);
void axpy_f64_c(double alpha, const double *x, double *y, size_t n);
void scale_f64_c(double alpha, double *x, size_t n);
double l2_norm_f64_c(const double *x, size_t n);
void matvec_f64_c(const double *matrix, const double *x, double *y, size_t rows, size_t cols);
void matmul_f64_c(const double *a, const double *b, double *c, size_t m, size_t k, size_t n);
int almost_equal(double a, double b, double eps);

#ifdef __cplusplus
}
#endif

#endif
