
#include <string.h>

#ifndef SR_MATRIX_H
#define SR_MATRIX_H

void
mat4_mul(float* out, float* A, float* B)
{
    memset(out, 0, 4 * 4 * sizeof(float));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            for (int i = 0; i < 4; i++) {
                float val =  A[c * 4 + i] * B[i * 4 + r];
                out[c * 4 + r] += val;
            }
        }
    }
}

void
mat4v_mul(float* out, float* A, float* v)
{
    memset(out, 0, 4 * sizeof(float));
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
                float val = A[c * 4 + r] * v[r];
                out[c] += val;
        }
    }
}

#endif  /* SR_MATRIX_H */
