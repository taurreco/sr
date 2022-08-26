
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_math.c"

/*********************************************************************
 *                                                                   *
 *                          unity helpers                            *
 *                                                                   *
 *********************************************************************/

void 
setUp() 
{
    /* empty */
}

void 
tearDown() 
{
    /* empty */
}
/*********************************************************************
 *                                                                   *
 *                             base case                             *
 *                                                                   *
 *********************************************************************/


/************
 * same_val *
 ************/

/* interpolate halfway between two scalar values. */

void 
mmul() 
{
    struct mat4 m1 = {
        2, 8, 1, 3,
        -10, 3, 5, 1,
        9, 9, 1, 2,
        7, 6, 10, 2
    };

    struct mat4 m2 = {
        3, 4, 10, 4,
        9, 2, 2, 2,
        1, 1, -1, 8,
        0, 12, 1, 4
    };

    struct mat4 ans = {
        79, 61, 38, 44,
        2, -17, -98, 10,
        109, 79, 109, 70,
        85, 74, 74, 128
    };

    matmul(&m1, &m2);
    float* d = (float*)(&m1);

    for (int i = 0; i < 16; i++) {
        if (i % 4 == 0)
            printf("\n");
        printf("%f ", d[i]);
    }

    TEST_ASSERT_EQUAL_MEMORY(&ans, &m1, sizeof(struct mat4));
}

void 
vmul() 
{
    struct mat4 m = {
        12, 3, 4, 19,
        -10, 2, 48, 2,
        8, 1, 0, 5,
        11, 9, 10, 6
    };

    float vec[4] = {
        5, 2, 3, 1,
    };

    float ans[4] = {
        97, 100, 47, 109
    };

    float out[4];

    matmul_v(out, &m, vec);

    for (int i = 0; i < 4; i++) {
        if (i % 4 == 0)
            printf("\n");
        printf("%f ", out[i]);
    }

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(ans, out, 4);
}

/*********************************************************************
 *                                                                   *
 *                              main                                 *
 *                                                                   *
 *********************************************************************/

int
main() 
{
    UNITY_BEGIN();
    RUN_TEST(mmul);
    RUN_TEST(vmul);
    return UNITY_END();
}

