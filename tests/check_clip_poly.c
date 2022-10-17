#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_clip.c"

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
 *                            base cases                             *
 *                                                                   *
 *********************************************************************/

/****************
 * tr_contained *
 ****************/

/* the triangle is entirely within the bounding box */
void
tr_contained()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        1, 0, 0, 1,
        0, 1, 0, 1,
        0, 0, 1, 1
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        1, 0, 0, 1,
        0, 1, 0, 1,
        0, 0, 1, 1
    };

    uint8_t clip_flags = 0;
    int num_pts = 3;

    clip_poly(src, &num_pts, 4, clip_flags);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(ans, src, num_pts * 4);
}

/*******************
 * tr_intersects_left *
 *******************/

/* the triangle intersects left plane of the bounding box */
void
tr_intersects_left()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -2, 0, 0, 1,
        0, -0.5, 0, 1,
        0, 0.5, 0, 1
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -1, 0.25, 0, 1,
        -1, -0.25, 0, 1,
        0, -0.5, 0, 1,
        0, 0.5, 0, 1
    };

    uint8_t clip_flags = SR_CLIP_LEFT_PLANE;
    int num_pts = 3;

    clip_poly(src, &num_pts, 4, clip_flags);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(ans, src, num_pts * 4);
}

/*******************************
 * tr_intersects_left_near *
 *******************************/

/* the triangle intersects left and near plane of the bounding box */
void
tr_intersects_left_near()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -1.25, 0, 0, 1,
        0, 0, -1.25, 1,
        0, 0, 0, 1
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -1, 0, 0, 1,
        -1, 0, -0.25, 1,
        -0.25, 0, -1, 1,
        0, 0, -1, 1,
        0, 0, 0, 1
    };

    uint8_t clip_flags = SR_CLIP_LEFT_PLANE | 
                        SR_CLIP_NEAR_PLANE;
    int num_pts = 3;
    
    clip_poly(src, &num_pts, 4, clip_flags);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(ans, src, num_pts * 4);
}

/*******************************
 * tr_intersects_top_left_bottom *
 *******************************/

/** 
 * the triangle intersects top, left, and bottom 
 * plane of the bounding box 
 */
void
tr_intersects_top_left_bottom()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -0.25, 1.25, -0.25, 1,
        -1.4, 0.2, -0.25, 1,
        0.2, -1.3, 0, 1
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        0.147059, -1, -0.029412, 1,
        -0.205882, 1, -0.225490, 1,
        -0.523809, 1, -0.25, 1,
        -1, 0.565217, -0.25, 1,
        -1, -0.175, -0.1875, 1,
        -0.12, -1, -0.05, 1
    };

    uint8_t clip_flags = SR_CLIP_TOP_PLANE | 
                         SR_CLIP_LEFT_PLANE |
                         SR_CLIP_BOTTOM_PLANE;
    int num_pts = 3;
    
    clip_poly(src, &num_pts, 4, clip_flags);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(ans, src, num_pts * 4);
}

/*********************
 * ln_intersects_top *
 *********************/

/* the line intersects the top plane of the bounding box */
void
ln_intersects_top()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        0, 2, 0, 1,
        0, 0, 0, 1
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        0, 1, 0, 1,
        0, 1, 0, 1,   /* duplicates points for lines whoops */
        0, 0, 0, 1,
    };

    uint8_t clip_flags = SR_CLIP_TOP_PLANE;
    int num_pts = 2;

    clip_poly(src, &num_pts, 4, clip_flags);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(ans, src, num_pts * 4);
}

/*********
 * fucky *
 *********/

/* is simply fucky */


int
winding_order(int winding_order, float* v0, float* v1, float* v2)
{
    float e01 = (v1[0] - v0[0]) * (v1[1] + v0[1]);
    float e12 = (v2[0] - v1[0]) * (v2[1] + v1[1]);
    float e20 = (v0[0] - v2[0]) * (v0[1] + v2[1]);

    return (e01 + e12 + e20) * winding_order > 0;  /* same sign */
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
    RUN_TEST(tr_contained);
    RUN_TEST(tr_intersects_left);
    RUN_TEST(tr_intersects_left_near);
    RUN_TEST(tr_intersects_top_left_bottom);
    RUN_TEST(ln_intersects_top);
    return UNITY_END();
}

