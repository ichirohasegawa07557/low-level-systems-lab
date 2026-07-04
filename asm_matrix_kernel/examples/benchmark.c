#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

int main(int argc, char **argv) {
    size_t n = argc > 1 ? (size_t)strtoull(argv[1], NULL, 10) : 1000000;
    double *a = (double *)malloc(sizeof(double) * n);
    double *b = (double *)malloc(sizeof(double) * n);
    double *y = (double *)malloc(sizeof(double) * n);
    if (!a || !b || !y) { fprintf(stderr, "allocation failed\n"); return 1; }
    for (size_t i = 0; i < n; ++i) {
        a[i] = (double)(i % 97) / 97.0;
        b[i] = (double)(i % 89) / 89.0;
        y[i] = 1.0;
    }
    double t0 = now_seconds();
    double c = dot_f64_c(a, b, n);
    double t1 = now_seconds();
    double s = dot_f64_asm(a, b, n);
    double t2 = now_seconds();
    axpy_f64_c(0.25, a, y, n);
    double t3 = now_seconds();
    double norm = l2_norm_f64_c(y, n);
    double t4 = now_seconds();
    printf("n=%zu\n", n);
    printf("dot_f64_c   = %.12f  time=%.6f sec\n", c, t1 - t0);
    printf("dot_f64_asm = %.12f  time=%.6f sec\n", s, t2 - t1);
    printf("axpy_f64_c  = completed time=%.6f sec\n", t3 - t2);
    printf("l2_norm_f64 = %.12f  time=%.6f sec\n", norm, t4 - t3);
    printf("match=%s\n", almost_equal(c, s, 1e-10) ? "yes" : "no");
    free(a); free(b); free(y);
    return almost_equal(c, s, 1e-10) ? 0 : 1;
}
