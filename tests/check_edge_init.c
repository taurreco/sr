
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_rast.c"

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
 *                             tests                                 *
 *                                                                   *
 *********************************************************************/

/*********************
 * outside_flat_edge *
 *********************/

/* p above flat top, going from v1 (left) to v2 (right) */

void 
outside_flat_edge() 
{

    float v1[2] = {1, 3};
    float v2[2] = {0, 3};
    float pt[2] = {0.5, 1};

    struct edge e12;
    float w0 = edge_init(&e12, v1, v2, pt);

    TEST_ASSERT_TRUE(e12.is_tl);
    TEST_ASSERT_EQUAL_FLOAT(0, e12.step_x);
    TEST_ASSERT_EQUAL_FLOAT(1, e12.step_y);
    TEST_ASSERT_EQUAL_FLOAT(-2, w0);
}


/********************
 * inside_flat_edge *
 ********************/

/* p below flat top, going from v1 (left) to v2 (right) */

void 
inside_flat_edge() 
{
    float v1[2] = {1, 3};
    float v2[2] = {0, 3};
    float pt[2] = {0.5, 4};

    struct edge e12;
    float w0 = edge_init(&e12, v1, v2, pt);

    TEST_ASSERT_TRUE(e12.is_tl);
    TEST_ASSERT_EQUAL_FLOAT(0, e12.step_x);
    TEST_ASSERT_EQUAL_FLOAT(1, e12.step_y);
    TEST_ASSERT_EQUAL_FLOAT(1, w0);
}

/********************
 * inside_left_edge *
 ********************/

/* p right of left edge */

void 
inside_left_edge() 
{
    float v2[2] = {2.3, 0.2};
    float v0[2] = {0.45, 4.2};
    float pt[2] = {2.7, 2.2};

    struct edge e20;
    float w1 = edge_init(&e20, v2, v0, pt);

    TEST_ASSERT_TRUE(e20.is_tl);
    TEST_ASSERT_EQUAL_FLOAT(4, e20.step_x);
    TEST_ASSERT_EQUAL_FLOAT(1.85, e20.step_y);
    TEST_ASSERT_EQUAL_FLOAT(5.3, w1);
}

/***********
 * on_edge *
 ***********/

/* p directly on edge */

void on_edge() {

    float v2[2] = {3, 2};
    float v0[2] = {2, 4};
    float pt[2] = {2.4, 3.2};

    struct edge e20;
    float w1 = edge_init(&e20, v2, v0, pt);

    TEST_ASSERT_TRUE(e20.is_tl);
    TEST_ASSERT_EQUAL_FLOAT(2, e20.step_x);
    TEST_ASSERT_EQUAL_FLOAT(1, e20.step_y);
    TEST_ASSERT_EQUAL_FLOAT(0, w1);
}

/*********************************************************************
 *                                                                   *
 *                             main                                  *
 *                                                                   *
 *********************************************************************/

int 
main() 
{
    UNITY_BEGIN();
    RUN_TEST(outside_flat_edge);
    RUN_TEST(inside_flat_edge);
    RUN_TEST(inside_left_edge);
    RUN_TEST(on_edge);
    return UNITY_END();
}

