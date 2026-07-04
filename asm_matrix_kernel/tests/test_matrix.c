#include "matrix.h"
#include <stdio.h>

static int check(int cond, const char *message) {
    if (!cond) {
        printf("test failed: %s\n", message);
        return 1;
    }
    return 0;
}

int main(void) {
    double a[4] = {1.0, 2.0, 3.0, 4.0};
    double b[4] = {5.0, 6.0, 7.0, 8.0};
    double expected = 70.0;
    double c = dot_f64_c(a, b, 4);
    double s = dot_f64_asm(a, b, 4);
    if (check(almost_equal(c, expected, 1e-12), "C dot product")) return 1;
    if (check(almost_equal(s, expected, 1e-12), "ASM dot product")) return 1;

    double y_axpy[4] = {1.0, 1.0, 1.0, 1.0};
    axpy_f64_c(2.0, a, y_axpy, 4);
    if (check(almost_equal(y_axpy[0], 3.0, 1e-12) && almost_equal(y_axpy[3], 9.0, 1e-12), "AXPY")) return 1;

    scale_f64_c(0.5, y_axpy, 4);
    if (check(almost_equal(y_axpy[0], 1.5, 1e-12) && almost_equal(y_axpy[3], 4.5, 1e-12), "scale")) return 1;

    if (check(almost_equal(l2_norm_f64_c(a, 4), 5.477225575051661, 1e-12), "L2 norm")) return 1;

    double matrix[6] = {1,2,3, 4,5,6};
    double x[3] = {1,1,1};
    double y[2] = {0,0};
    matvec_f64_c(matrix, x, y, 2, 3);
    if (check(almost_equal(y[0], 6.0, 1e-12) && almost_equal(y[1], 15.0, 1e-12), "matrix-vector multiplication")) return 1;

    double m1[6] = {1,2,3, 4,5,6};
    double m2[6] = {7,8, 9,10, 11,12};
    double out[4] = {0,0,0,0};
    matmul_f64_c(m1, m2, out, 2, 3, 2);
    if (check(almost_equal(out[0], 58.0, 1e-12) && almost_equal(out[3], 154.0, 1e-12), "matrix-matrix multiplication")) return 1;

    printf("asm_matrix_kernel tests passed\n");
    return 0;
}
