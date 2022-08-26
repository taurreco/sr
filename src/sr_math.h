
#ifndef SR_MATH_H
#define SR_MATH_H

#include <string.h>
#include <math.h>
#include "sr.h"

struct mat4 {
    float e00, e01, e02, e03;
    float e10, e11, e12, e13;
    float e20, e21, e22, e23;
    float e30, e31, e32, e33;
};

extern void 
matmul(struct mat4* a, struct mat4* b);

extern void
matmul_v(float* a, struct mat4* b, float* c);

extern void
cross(float* a, float* b, float* c);

extern void
normalize(float* a);

#endif   /* SR_MATH_H */