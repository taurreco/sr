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

/*************
 * contained *
 *************/

/* the point is entirely within the bounding box */
void
contained()
{
    float pt[4] = {
        0, 0, 0, 1
    };
    uint8_t clip_flag;
    clip_test(pt, &clip_flag);
    TEST_ASSERT_EQUAL_UINT8(0, clip_flag);
}

/********
 * left *
 ********/

/* the point is only beyond the left of the bounding box */
void
left()
{
    float pt[4] = {
        -2, 0, 0, 1
    };
    uint8_t clip_flag;
    clip_test(pt, &clip_flag);
    TEST_ASSERT_EQUAL_UINT8(SR_CLIP_LEFT_PLANE, clip_flag);
}

/**********
 * bottom *
 **********/

/* the point is only beyond the bottom of the bounding box */
void
bottom()
{
    float pt[4] = {
        0, -2, 0, 1
    };
    uint8_t clip_flag;
    clip_test(pt, &clip_flag);
    TEST_ASSERT_EQUAL_UINT8(SR_CLIP_BOTTOM_PLANE, clip_flag);
}

/********
 * near *
 ********/

/* the point is only beyond the near plane of the bounding box */
void
near()
{
    float pt[4] = {
        0, 0, -2, 1
    };
    uint8_t clip_flag;
    clip_test(pt, &clip_flag);
    TEST_ASSERT_EQUAL_UINT8(SR_CLIP_NEAR_PLANE, clip_flag);
}

/*********
 * right *
 *********/

/* the point is only beyond the right of the bounding box */
void
right()
{
    float pt[4] = {
        2, 0, 0, 1
    };
    uint8_t clip_flag;
    clip_test(pt, &clip_flag);
    TEST_ASSERT_EQUAL_UINT8(SR_CLIP_RIGHT_PLANE, clip_flag);
}

/*******
 * top *
 *******/

/* the point is only beyond the top of the bounding box */
void
top()
{
    float pt[4] = {
        0, 2, 0, 1
    };
    uint8_t clip_flag;
    clip_test(pt, &clip_flag);
    TEST_ASSERT_EQUAL_UINT8(SR_CLIP_TOP_PLANE, clip_flag);
}

/*********************************************************************
 *                                                                   *
 *                         multiple planes                           *
 *                                                                   *
 *********************************************************************/

/****************
 * top_and_left *
 ****************/

/** 
 * the point is only beyond the left and 
 * right of the bounding box 
 */
void
top_and_left()
{
    float pt[4] = {
        -3, 2, 0, 1
    };
    uint8_t clip_flag;
    clip_test(pt, &clip_flag);
    TEST_ASSERT_EQUAL_UINT8(SR_CLIP_TOP_PLANE | 
                            SR_CLIP_LEFT_PLANE, 
                            clip_flag);
}

/*******************
 * bottom_and_near *
 *******************/

/** 
 * the point is only beyond the bottom and near 
 * plane of the bounding box
 */
void
bottom_and_near()
{
    float pt[4] = {
        0, -11, -2, 1
    };
    uint8_t clip_flag;
    clip_test(pt, &clip_flag);
    TEST_ASSERT_EQUAL_UINT8(SR_CLIP_BOTTOM_PLANE | 
                            SR_CLIP_NEAR_PLANE, 
                            clip_flag);
}

/*************
 * on_corner *
 *************/

/* the point is on the corner of the bounding box */
void
on_corner()
{
    float pt[4] = {
        1, 1, 1, 1
    };
    uint8_t clip_flag;
    clip_test(pt, &clip_flag);
    TEST_ASSERT_EQUAL_UINT8(0, clip_flag);
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
    RUN_TEST(contained);
    RUN_TEST(left);
    RUN_TEST(bottom);
    RUN_TEST(near);
    RUN_TEST(right);
    RUN_TEST(top);
    RUN_TEST(top_and_left);
    RUN_TEST(bottom_and_near);
    RUN_TEST(on_corner);

    return UNITY_END();
}

