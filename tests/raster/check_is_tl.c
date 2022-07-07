
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
 *                          trivial cases                            *
 *                                                                   *
 *********************************************************************/

/****************
 * flat_top_ccw *
 ****************/

/* flat edge directed left */

void 
flat_top_ccw() 
{
    float v1[2] = {1, 0};
    float v2[2] = {0, 0};

    TEST_ASSERT_TRUE(is_tl(v1, v2));
}

/***************
 * flat_top_cw *
 ***************/

/* flat edge directed right */

void
flat_top_cw()
{
    float v1[2] = {1, 0};
    float v2[2] = {0, 0};

    TEST_ASSERT_FALSE(is_tl(v2, v1));
}

/*********************
 * vertical_edge_ccw *
 *********************/

/* vertical edge directed down */

void 
vertical_edge_ccw() 
{
    float v2[2] = {0, 0};
    float v0[2] = {0, 1};

    TEST_ASSERT_TRUE(is_tl(v2, v0));
}

/********************
 * vertical_edge_cw *
 ********************/

/* vertical edge directed up */

void
vertical_edge_cw()
{
    float v2[2] = {0, 0};
    float v0[2] = {0, 1};

    TEST_ASSERT_FALSE(is_tl(v0, v2));    
}

/*********************************************************************
 *                                                                   *
 *                        invariant cases                            *
 *                                                                   *
 *********************************************************************/

/*************
 * flat_long *
 *************/

/* length invariant */

void
flat_long()
{
    float v1[2] = {5.2, 0};
    float v2[2] = {1, 0};

    TEST_ASSERT_TRUE(is_tl(v1, v2));
}

/*******************
 * negative_coords *
 *******************/

/* both points are not in clip space, has no affect on output */

void
negative_coords()
{

    float v1[2] = {-1000.23, -472};
    float v2[2] = {-1, 90};

    TEST_ASSERT_TRUE(is_tl(v1, v2));
}

/********************
 * other_attributes *
 ********************/

/* attributes beyond first two are garbage */

void
other_attributes()
{
    float v1[3] = {12.03, 43.1, 291.10};
    float v2[3] = {-0.21, 43.1, 1.0};

    TEST_ASSERT_TRUE(is_tl(v1, v2));

    float v1_diff[3] = {12.03, 43.1, -387};
    float v2_diff[3] = {-0.21, 43.1, 198.3001};

    TEST_ASSERT_TRUE(is_tl(v1_diff, v2_diff));
}

/*********************************************************************
 *                                                                   *
 *                      slanted edge cases                           *
 *                                                                   *
 *********************************************************************/

/********************
 * slanted_down_ccw *
 ********************/

/* from left to right, the edge goes from high to low directed up */

void 
slanted_down_ccw() 
{
    float v1[2] = {1, 1};
    float v2[2] = {0, 0};

    TEST_ASSERT_FALSE(is_tl(v1, v2));
}

/*******************
 * slanted_down_cw *
 *******************/

/* from left to right, the edge goes from high to low directed down */

void slanted_down_cw()
{
    float v1[2] = {1, 1};
    float v2[2] = {0, 0};

    TEST_ASSERT_TRUE(is_tl(v2, v1));
}

/******************
 * slanted_up_ccw *
 ******************/

/* from left to right, the edge goes from low to high directed down */

void 
slanted_up_ccw()
{
    float v2[2] = {1, 0};
    float v0[2] = {0, 1};

    TEST_ASSERT_TRUE(is_tl(v2, v0));
}

/******************
 * slanted_up_cw *
 ******************/

/* from left to right, the edge goes from low to high directed up */

void
slanted_up_cw()
{
    float v2[2] = {1, 0};
    float v0[2] = {0, 1};

    TEST_ASSERT_FALSE(is_tl(v0, v2));
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
    RUN_TEST(flat_top_ccw);
    RUN_TEST(flat_top_cw);
    RUN_TEST(vertical_edge_ccw);
    RUN_TEST(vertical_edge_cw);
    RUN_TEST(flat_long);
    RUN_TEST(negative_coords);
    RUN_TEST(other_attributes);
    RUN_TEST(slanted_down_ccw);
    RUN_TEST(slanted_down_cw);
    RUN_TEST(slanted_up_ccw);
    RUN_TEST(slanted_up_cw);
    return UNITY_END();
}

