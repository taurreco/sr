
#include <math.h>
#include "sr_math.h"

/**
 * sr_math.c
 * --------
 * provides an internal matrix representation (mat4)
 * and associated operations for the sr lib
 * 
 */

/*********************************************************************
 *                                                                   *
 *                       private definitions                         *
 *                                                                   *
 *********************************************************************/

/**********
 * matmul *
 **********/

/* multiplies two 4x4 matrices, a and b, stores the result in a */
extern void 
matmul(struct mat4* a, struct mat4* b)
{
    struct mat4 tmp;
    tmp.e00 = a->e00 * b->e00 + a->e01 * b->e10 + a->e02 * b->e20 + a->e03 * b->e30;
    tmp.e01 = a->e00 * b->e01 + a->e01 * b->e11 + a->e02 * b->e21 + a->e03 * b->e31; 
    tmp.e02 = a->e00 * b->e02 + a->e01 * b->e12 + a->e02 * b->e22 + a->e03 * b->e32; 
    tmp.e03 = a->e00 * b->e03 + a->e01 * b->e13 + a->e02 * b->e23 + a->e03 * b->e33; 
    tmp.e10 = a->e10 * b->e00 + a->e11 * b->e10 + a->e12 * b->e20 + a->e13 * b->e30; 
    tmp.e11 = a->e10 * b->e01 + a->e11 * b->e11 + a->e12 * b->e21 + a->e13 * b->e31; 
    tmp.e12 = a->e10 * b->e02 + a->e11 * b->e12 + a->e12 * b->e22 + a->e13 * b->e32; 
    tmp.e13 = a->e10 * b->e03 + a->e11 * b->e13 + a->e12 * b->e23 + a->e13 * b->e33; 
    tmp.e20 = a->e20 * b->e00 + a->e21 * b->e10 + a->e22 * b->e20 + a->e23 * b->e30; 
    tmp.e21 = a->e20 * b->e01 + a->e21 * b->e11 + a->e22 * b->e21 + a->e23 * b->e31; 
    tmp.e22 = a->e20 * b->e02 + a->e21 * b->e12 + a->e22 * b->e22 + a->e23 * b->e32; 
    tmp.e23 = a->e20 * b->e03 + a->e21 * b->e13 + a->e22 * b->e23 + a->e23 * b->e33; 
    tmp.e30 = a->e30 * b->e00 + a->e31 * b->e10 + a->e32 * b->e20 + a->e33 * b->e30; 
    tmp.e31 = a->e30 * b->e01 + a->e31 * b->e11 + a->e32 * b->e21 + a->e33 * b->e31; 
    tmp.e32 = a->e30 * b->e02 + a->e31 * b->e12 + a->e32 * b->e22 + a->e33 * b->e32; 
    tmp.e33 = a->e30 * b->e03 + a->e31 * b->e13 + a->e32 * b->e23 + a->e33 * b->e33;
    *a = tmp;
};

/************
 * matmul_v *
 ************/

/* applys the matrix b to vector c, stores result in vector a */ 
extern void
matmul_v(float* a, struct mat4* b, float* c)
{
    a[0] = c[0] * b->e00 + c[1] * b->e01 + c[2] * b->e02 + c[3] * b->e03;
    a[1] = c[0] * b->e10 + c[1] * b->e11 + c[2] * b->e12 + c[3] * b->e13;
    a[2] = c[0] * b->e20 + c[1] * b->e21 + c[2] * b->e22 + c[3] * b->e23;
    a[3] = c[0] * b->e30 + c[1] * b->e31 + c[2] * b->e32 + c[3] * b->e33;
}

/*********
 * cross *
 *********/

/* applys the cross product b x c, stores result in a */
extern void
cross(float* a, float* b, float* c)
{
    a[0] = (b[1] * c[2]) - (b[2] * c[1]);
    a[1] = (b[2] * c[0]) - (b[0] * c[2]);
    a[2] = (b[0] * c[1]) - (b[1] * c[0]);
}

/*************
 * normalize *
 *************/

/* normalizes vector3 in place */
extern void
normalize(float* a)
{
    float m = sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
    a[0] = a[0] / m;
    a[1] = a[1] / m;
    a[2] = a[2] / m;
}