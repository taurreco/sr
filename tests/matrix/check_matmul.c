
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_matrix.h"

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
    float mat1[4 * 4] = {
        2, 8, 1, 3,
        -10, 3, 5, 1,
        9, 9, 1, 2,
        7, 6, 10, 2
    };

    float mat2[4 * 4] = {
        3, 4, 10, 4,
        9, 2, 2, 2,
        1, 1, -1, 8,
        0, 12, 1, 4
    };

    float ans[4 * 4] = {
        79, 61, 38, 44,
        2, -17, -98, 10,
        109, 79, 109, 70,
        85, 74, 74, 128
    };

    float out[4 * 4];

    mat4_mul(out, mat1, mat2);

    for (int i = 0; i < 16; i++) {
        if (i % 4 == 0)
            printf("\n");
        printf("%f ", out[i]);
    }

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(ans, out, 4 * 4);
}

void 
vmul() 
{
    float mat1[4 * 4] = {
        12, 3, 4, 19,
        -10, 2, 48, 2,
        8, 1, 0, 5,
        11, 9, 10, 6
    };

    float mat2[4] = {
        5, 2, 3, 1,
    };

    float ans[4] = {
        97, 100, 47, 109
    };

    float out[4];

    mat4v_mul(out, mat1, mat2);

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

