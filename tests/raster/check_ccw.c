


#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_raster.h"

/*********************************************************************
 *                                                                   *
 *                           unity helpers                           *
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
 *                              tests                                *
 *                                                                   *
 *********************************************************************/

/************
 * identity *
 ************/

/* the inputs are already in correct order and nothing needs to be done */

void
identity() 
{
    float v0_arr[2] = {1, 2};
    float v1_arr[2] = {1, 0};
    float v2_arr[2] = {0, 1};

    float old_v0[2] = {1, 2};
    float old_v1[2] = {1, 0};
    float old_v2[2] = {0, 1};

    float* v0 = (float*)v0_arr;
    float* v1 = (float*)v1_arr;
    float* v2 = (float*)v2_arr;

    ccw(&v0, &v1, &v2);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v0, v0, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v1, v1, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v2, v2, 2);
}

/*****************
 * v2_left_of_v1 *
 *****************/

/* v2 is moved to be left of v1 when it starts right of v1 */

void
v1_left_v2() 
{
    float v0_arr[2] = {1, 2};
    float v1_arr[2] = {0, 1};
    float v2_arr[2] = {1, 0};    /* right of v1 */

    float old_v0[2] = {1, 2};
    float old_v1[2] = {0, 1};
    float old_v2[2] = {1, 0};

    float* v0 = (float*)v0_arr;
    float* v1 = (float*)v1_arr;
    float* v2 = (float*)v2_arr;

    ccw(&v0, &v1, &v2);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v0, v0, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v1, v2, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v2, v1, 2);
}

/*************
 * v0_top_cw *
 *************/

/* output v0 is must be the lowest point */

void
v0_top_cw() 
{
    float v0_arr[2] = {1, 0};
    float v1_arr[2] = {0, 1};
    float v2_arr[2] = {1, 2};

    float old_v0[2] = {1, 0};
    float old_v1[2] = {0, 1};
    float old_v2[2] = {1, 2};

    float* v0 = (float*)v0_arr;
    float* v1 = (float*)v1_arr;
    float* v2 = (float*)v2_arr;

    ccw(&v0, &v1, &v2);

    /* test */

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v0, v1, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v1, v2, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v2, v0, 2);
}

/**************
 * v0_top_ccw *
 **************/

/* output v0 changed to lowest even if input is already ccw oriented */

void
v0_top_ccw () 
{
    float v0_arr[2] = {1, 0};
    float v1_arr[2] = {1, 2};
    float v2_arr[2] = {0, 1};

    float old_v0[2] = {1, 0};
    float old_v1[2] = {1, 2};
    float old_v2[2] = {0, 1};

    float* v0 = (float*)v0_arr;
    float* v1 = (float*)v1_arr;
    float* v2 = (float*)v2_arr;
    
    ccw(&v0, &v1, &v2);

    /* test */

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v0, v1, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v1, v0, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v2, v2, 2);
}

/*********************
 * v2_v0_flat_bottom *
 *********************/

/* v2 must be right of v0 */

void 
v2_v0_flat_bottom() 
{
    /* v2 (left) and v0 (right) are bottommost */

    float v0_arr[2] = {2, 2};
    float v1_arr[2] = {1, 0};
    float v2_arr[2] = {0, 2};

    float old_v0[2] = {2, 2};
    float old_v1[2] = {1, 0};
    float old_v2[2] = {0, 2};

    float* v0 = (float*)v0_arr;
    float* v1 = (float*)v1_arr;
    float* v2 = (float*)v2_arr;

    ccw(&v0, &v1, &v2);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v0, v0, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v1, v1, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v2, v2, 2);
}

/*********************
 * v0_v1_flat_bottom *
 *********************/

/* v0 must be left of v1 */

void 
v0_v1_flat_bottom() 
{
    /* v0 and v1 are bottommost */

    float v0_arr[2] = {0, 2};
    float v1_arr[2] = {2, 2};
    float v2_arr[2] = {1, 0};

    float old_v0[2] = {0, 2};
    float old_v1[2] = {2, 2};
    float old_v2[2] = {1, 0};

    float* v0 = (float*)v0_arr;
    float* v1 = (float*)v1_arr;
    float* v2 = (float*)v2_arr;

    ccw(&v0, &v1, &v2);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v0, v0, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v1, v1, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v2, v2, 2);
}

/**********************
 * v0_v2_flat_bottom    *
 **********************/

/* v2 must be right of v0 */

void 
v0_v2_flat_bottom() 
{
    float v0_arr[2] = {0, 2};
    float v1_arr[2] = {1, 0};
    float v2_arr[2] = {2, 2};

    float old_v0[2] = {0, 2};
    float old_v1[2] = {1, 0};
    float old_v2[2] = {2, 2};

    float* v0 = (float*)v0_arr;
    float* v1 = (float*)v1_arr;
    float* v2 = (float*)v2_arr;

    ccw(&v0, &v1, &v2);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v0, v0, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v1, v2, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v2, v1, 2);
}

/**********************
 * v1_v0_flat_bottom    *
 **********************/

/* v0 must be left of v1 */

void 
v1_v0_flat_bottom() 
{
    float v0_arr[2] = {2, 2};
    float v1_arr[2] = {0, 2};
    float v2_arr[2] = {1, 0};

    float old_v0[2] = {2, 2};
    float old_v1[2] = {0, 2};
    float old_v2[2] = {1, 0};

    float* v0 = (float*)v0_arr;
    float* v1 = (float*)v1_arr;
    float* v2 = (float*)v2_arr;

    ccw(&v0, &v1, &v2);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v0, v0, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v1, v2, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v2, v1, 2);
}

/********************
 * v2_v1_flat_right *
 ********************/

/* v1 lower than v2 if x-aligned */

void v2_v1_flat_right() {

    float v0_arr[2] = {1, 2};
    float v1_arr[2] = {3, 0};
    float v2_arr[2] = {3, 1};

    float old_v0[2] = {1, 2};
    float old_v1[2] = {3, 0};
    float old_v2[2] = {3, 1};

    float* v0 = (float*)v0_arr;
    float* v1 = (float*)v1_arr;
    float* v2 = (float*)v2_arr;

    ccw(&v0, &v1, &v2);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v0, v0, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v1, v2, 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v2, v1, 2);
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
    RUN_TEST(identity);
    RUN_TEST(v1_left_v2);
    RUN_TEST(v0_top_cw);
    RUN_TEST(v0_top_ccw);
    RUN_TEST(v2_v0_flat_bottom);
    RUN_TEST(v0_v1_flat_bottom);
    RUN_TEST(v0_v2_flat_bottom);
    RUN_TEST(v1_v0_flat_bottom);
    RUN_TEST(v2_v1_flat_right);
    return UNITY_END();
}
