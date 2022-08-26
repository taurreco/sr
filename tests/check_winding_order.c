
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_render.c"

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
 *                              ccw cases                            *
 *                                                                   *
 *********************************************************************/

/*************
 * ccw_basic *
 *************/

/* standard ccw */

void 
ccw_basic() 
{
    float v0[4] = {
        0, 4, 1, 1
    };

    float v1[4] = {
        2, 0, 1, 1
    };

    float v2[4] = {
        0, 0, 1, 1
    };

    TEST_ASSERT_TRUE(winding_order(SR_WINDING_ORDER_CCW, v0, v1, v2));
    TEST_ASSERT_FALSE(winding_order(SR_WINDING_ORDER_CW, v0, v1, v2));
}

/**************
 * ccw_v0_top *
 **************/

/* ccw triangle with v0 top */

void 
ccw_v0_top() 
{
    float v2[4] = {
        0, 4, 1, 1
    };

    float v0[4] = {
        2, 0, 1, 1
    };

    float v1[4] = {
        0, 0, 1, 1
    };

    TEST_ASSERT_TRUE(winding_order(SR_WINDING_ORDER_CCW, v0, v1, v2));
    TEST_ASSERT_FALSE(winding_order(SR_WINDING_ORDER_CW, v0, v1, v2));
}

/********************
 * ccw_diff_heights *
 ********************/

/* ccw triangle whose vertices never share a y coord */

void 
ccw_diff_heights() 
{
    float v1[4] = {
        3, 4, 1, 1
    };

    float v0[4] = {
        10, 1, 1, 1
    };

    float v2[4] = {
        6, 7, 1, 1
    };

    TEST_ASSERT_TRUE(winding_order(SR_WINDING_ORDER_CCW, v0, v1, v2));
    TEST_ASSERT_FALSE(winding_order(SR_WINDING_ORDER_CW, v0, v1, v2));
}


/*********************************************************************
 *                                                                   *
 *                              cw cases                             *
 *                                                                   *
 *********************************************************************/

/*************
 * cw_basic *
 *************/

/* standard cw */

void 
cw_basic() 
{
    float v0[4] = {
        0, 4, 1, 1
    };

    float v2[4] = {
        2, 0, 1, 1
    };

    float v1[4] = {
        0, 0, 1, 1
    };

    TEST_ASSERT_FALSE(winding_order(SR_WINDING_ORDER_CCW, v0, v1, v2));
    TEST_ASSERT_TRUE(winding_order(SR_WINDING_ORDER_CW, v0, v1, v2));
}

/**************
 * cw_v0_top *
 **************/

/* cw triangle with v0 top */

void 
cw_v0_top() 
{
    float v1[4] = {
        0, 4, 1, 1
    };

    float v0[4] = {
        2, 0, 1, 1
    };

    float v2[4] = {
        0, 0, 1, 1
    };

    TEST_ASSERT_FALSE(winding_order(SR_WINDING_ORDER_CCW, v0, v1, v2));
    TEST_ASSERT_TRUE(winding_order(SR_WINDING_ORDER_CW, v0, v1, v2));
}

/*******************
 * cw_diff_heights *
 *******************/

/* cw triangle whose vertices never share a y coord */

void 
cw_diff_heights() 
{
    float v2[4] = {
        3, 4, 1, 1
    };

    float v0[4] = {
        10, 1, 1, 1
    };

    float v1[4] = {
        6, 7, 1, 1
    };

    TEST_ASSERT_FALSE(winding_order(SR_WINDING_ORDER_CCW, v0, v1, v2));
    TEST_ASSERT_TRUE(winding_order(SR_WINDING_ORDER_CW, v0, v1, v2));
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
    RUN_TEST(ccw_basic);
    RUN_TEST(ccw_v0_top);
    RUN_TEST(ccw_diff_heights);
    RUN_TEST(cw_basic);
    RUN_TEST(cw_v0_top);
    RUN_TEST(cw_diff_heights);
    return UNITY_END();
}

