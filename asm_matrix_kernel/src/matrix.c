#include "matrix.h"
#include <math.h>
#include <stddef.h>

#ifndef HAVE_ASM_DOT
#define HAVE_ASM_DOT 0
#endif

double dot_f64_c(const double *a, const double *b, size_t n) {
    double acc = 0.0;
    for (size_t i = 0; i < n; ++i) acc += a[i] * b[i];
    return acc;
}

#if !HAVE_ASM_DOT
double dot_f64_asm(const double *a, const double *b, size_t n) {
    return dot_f64_c(a, b, n);
}
#endif

void vec_add_f64_c(const double *a, const double *b, double *out, size_t n) {
    for (size_t i = 0; i < n; ++i) out[i] = a[i] + b[i];
}

void axpy_f64_c(double alpha, const double *x, double *y, size_t n) {
    for (size_t i = 0; i < n; ++i) y[i] += alpha * x[i];
}

void scale_f64_c(double alpha, double *x, size_t n) {
    for (size_t i = 0; i < n; ++i) x[i] *= alpha;
}

double l2_norm_f64_c(const double *x, size_t n) {
    return sqrt(dot_f64_c(x, x, n));
}

void matvec_f64_c(const double *matrix, const double *x, double *y, size_t rows, size_t cols) {
    for (size_t r = 0; r < rows; ++r) {
        y[r] = dot_f64_c(matrix + r * cols, x, cols);
    }
}

void matmul_f64_c(const double *a, const double *b, double *c, size_t m, size_t k, size_t n) {
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double acc = 0.0;
            for (size_t p = 0; p < k; ++p) {
                acc += a[i * k + p] * b[p * n + j];
            }
            c[i * n + j] = acc;
        }
    }
}

int almost_equal(double a, double b, double eps) {
    return fabs(a - b) <= eps * (1.0 + fabs(a) + fabs(b));
}
