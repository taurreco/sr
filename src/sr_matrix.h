
#include <string.h>

#ifndef SR_MATRIX_H
#define SR_MATRIX_H

void
matmul(float* dest, float* A, float* B)
{
    memset(dest, 0, 4 * 4 * sizeof(float));
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            for (int i = 0; i < 4; i++) {
                float val =  A[c * 4 + i] * B[i * 4 + r];
                dest[c * 4 + r] += val;
            }
        }
    }
}

void
matmul_v(float* dest, float* A, float* v)
{
    memset(dest, 0, 4 * sizeof(float));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
                float val = A[c * 4 + r] * v[r];
                dest[c] += val;
        }
    }
}

#endif  /* SR_MATRIX_H */
