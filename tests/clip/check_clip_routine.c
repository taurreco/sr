#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_clip.h"

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

/**************
 * clip_no_pts *
 **************/

/**************************************
 *     -15, 15           15,15        *
 *        y _______________           *
 *         |              |           *
 *         |      v1      |           *
 *         |     /  \     |           *
 *         |   v2__  \    |           *
 *         |       -- v0  |           *
 *         |              |           *
 *         |______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/

/* v2 is the only vertex outside the negative x clip plane */
void 
clip_no_pts()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        10, -11, 0, 15,       /* v0 */
        2, 10, 0, 15,         /* v1 */
        -5, 2, 0, 15,         /* v2 */
    };

    float* ans = (float*)src;
    float dest[16 * SR_MAX_ATTRIBUTE_COUNT];

    size_t num_pts = 3;
    clip_routine(src, dest, &num_pts, 4, 0, -1);

    printf("num_pts %ld", num_pts);

    TEST_ASSERT_EQUAL_UINT(num_pts, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4 * 3);
}

/**************
 * cull_triangle *
 **************/

/**************************************
 *     -15, 15           15,15        *
 *        y _______________           *
 *         |              |           *
 *         |              |           *
 *  v2--v1 |              |           *
 *   \  |  |              |           *
 *    \ |  |              |           *
 *     v0  |              |           *
 *         |______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/

/* v2 is the only vertex outside the negative x clip plane */
void 
cull_triangle()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -20, -10, 0, 15,       /* v0 */
        -20, 2, 0, 15,         /* v1 */
        -26, 2, 0, 15       /* v2 */
    };

    float dest[16 * SR_MAX_ATTRIBUTE_COUNT];

    size_t num_pts = 3;
    clip_routine(src, dest, &num_pts, 4, 0, -1);

    TEST_ASSERT_EQUAL_UINT(num_pts, 0);
}

/*********************************************************************
 *                                                                   *
 *                     number of points clipped                      *
 *                                                                   *
 *********************************************************************/

/**************
 * clip_one_pt *
 **************/

/**************************************
 *     -15, 15           15,15        *
 *        y _______________           *
 *         |              |           *
 *         |        _ - v1|           *
 *         |    . -       | \         *
 *      _ ,|- `           |   \       *
 *   v2----|--------------|----v0     *
 *         |              |           *
 *         |______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/

/**
 * clipping away only the point outside the negatve 
 * x face of bounding box
 */
void 
clip_one_pt()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        20, 0, 0, 15,     /* v0 */
        15, 10, 0, 15,    /* v1 */
        -20, 0, 0, 15     /* v2 */
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -15, 0, 0, 15,
        20, 0, 0, 15,
        15, 10, 0, 15,
        -15, 1.428572, 0, 15
    };

    float dest[16 * SR_MAX_ATTRIBUTE_COUNT];

    size_t num_pts = 3;

    clip_routine(src, dest, &num_pts, 4, 0, -1);

    TEST_ASSERT_EQUAL_UINT(num_pts, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4 * 4);
}

/****************
 * clip_two_pts *
 ****************/

/**************************************
 *     -15, 15           15,15        *
 *        y _______________           *
 *         |              |           *
 *     v2-.|              |           *
 *      |  |`-- _         |           *
 *      |  |     'v1  c   |           *
 *      |  |_--'          |           *
 *     v0_-|              |           *
 *         |______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/

/* two points are outside of triangle */
void 
clip_two_pts()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -20, -11, 0, 15,     /* v0 */
        0, 0, 0, 15,         /* v1 */
        -20, 10, 0, 15       /* v2 */
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -15, -8.25, 0, 15,
        0, 0, 0, 15,
        -15, 7.5, 0, 15,
    };

    float dest[16 * SR_MAX_ATTRIBUTE_COUNT];

    size_t num_pts = 3;
    clip_routine(src, dest, &num_pts, 4, 0, -1);

    TEST_ASSERT_EQUAL_UINT(num_pts, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4 * 3);
}

/******************
 * clip_three_pts *
 ******************/

/**************************************
 *     -15, 15    v1      15,15       *
 *        y _____/__\______           *
 *         |    /    \    |           *
 *         |   /      \   |           *
 *         |  /        \  |           *
 *         | /          \ |           *
 *         |/             |\          *
 *        /|_______-------| v0        *
 *      v2-|______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/

/* triangle is larger than bounding box, but still inside it */
void 
clip_three_pts()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        17, -13, 0, 15,       /* v0 */
        0, 19, 0, 15,         /* v1 */
        -20, -14, 0, 15       /* v2 */
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -15, -13.864865, 0, 15,
        15, -13.054054, 0, 15,
        15, -9.235294, 0, 15,
        2.125, 15, 0, 15,
        -2.424242, 15, 0, 15,
        -15, -5.75, 0, 15
    };

    float dest[16 * SR_MAX_ATTRIBUTE_COUNT];
    float tmp[16 * SR_MAX_ATTRIBUTE_COUNT];

    size_t num_pts = 3;
    clip_routine(src, dest, &num_pts, 4, 0, -1);  /* clip left */
    clip_routine(dest, tmp, &num_pts, 4, 1, 1);       /* clip top */
    clip_routine(tmp, dest, &num_pts, 4, 0, 1);       /* clip right */

    TEST_ASSERT_EQUAL_UINT(num_pts, 6);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4 * 6);
}

/*********************************************************************
 *                                                                   *
 *                     attribute interpolation                       *
 *                                                                   *
 *********************************************************************/

/*************
 * clip_attr *
 *************/

/**************************************
 *     -15, 15           15,15        *
 *        y _______________           *
 *         |              |           *
 *     v2-.|              |           *
 *      |  |`-- _         |           *
 *      |  |     'v1  c   |           *
 *      |  |_--'          |           *
 *     v0_-|              |           *
 *         |______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/

/* each point has a lot of data */
void 
clip_attr()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -20, -11, 0, 15, 1082, 1234, 91, 4,    /* v0 */
        0, 0, 0, 15, 2845, 1230, 23, 4,        /* v1 */
        -20, 10, 0, 15, 1920, 59, 180, 1       /* v2 */
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -15, -8.25, 0, 15, 1522.75, 1233, 74, 4,
        0, 0, 0, 15, 2845, 1230, 23, 4,
        -15, 7.5, 0, 15, 2151.25, 351.75, 140.75, 1.75
    };

    float dest[16 * SR_MAX_ATTRIBUTE_COUNT];

    size_t num_pts = 3;
    clip_routine(src, dest, &num_pts, 8, 0, -1);

    TEST_ASSERT_EQUAL_UINT(num_pts, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 8 * 3);
}


/*********************************************************************
 *                                                                   *
 *                            misc cases                             *
 *                                                                   *
 *********************************************************************/

/*************
 * miss_axes *
 *************/

/**************************************
 *     -15, 15    v1      15,15       *
 *        z _____/__\______           *
 *         |    /    \    |           *
 *         |   /      \   |           *
 *         |  /        \  |           *
 *         | /          \ |           *
 *         |/             |\          *
 *        /|_______-------| v0        *
 *      v2-|______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/

/* don't even worry about the x or y axes */
void 
miss_axes()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        17, 2, -13, 15,         /* v0 */
        0, -11, 19, 15,         /* v1 */
        -20, 200, -14, 15       /* v2 */
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        17, 2, -13, 15,
        2.125, -9.375, 15, 15,
        -2.424242, 14.575758, 15, 15,
        -20, 200, -14, 15
    };

    float dest[16 * SR_MAX_ATTRIBUTE_COUNT];

    size_t num_pts = 3;
    clip_routine(src, dest, &num_pts, 4, 2, 1);  /* clip left */

    TEST_ASSERT_EQUAL_UINT(num_pts, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4 * 4);
}

/*******************
 * clip_pt_on_edge *
 *******************/

/**************************************
 *     -15, 15           15,15        *
 *        y _______________           *
 *         |              |           *
 *         |       _ , -v2|<--on edge *
 *         |   _ -        | \         *
 *         |v3            |   \       *
 *         |v0------------|----v1     *
 *         |              |           *
 *         |______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/

/**
 * will produce a degenerate triangle when a point 
 * outside the box connects to a point exactly on the box  
 * note: degenerate triangles are still correct-- 
 * they just waste computation, but this case is so rare,
 * checking for it every time is a bigger waste
 * (I think...) 
 */

void 
clip_pt_on_edge()
{
    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -15, 0, 0, 15,           /* v0 */
        20, 0, 0, 15,            /* v1 */
        15, 10, 0, 15,           /* v2 */
        -15, 1.428572, 0, 15     /* v3 */
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -15, 0, 0, 15,
        15, 0, 0, 15,
        15, 10, 0, 15,     /* degenerate triangle edge case */
        15, 10, 0, 15,
        -15, 1.428572, 0, 15
    };

    float dest[16 * SR_MAX_ATTRIBUTE_COUNT];

    size_t num_pts = 4;

    clip_routine(src, dest, &num_pts, 4, 0, 1);

    TEST_ASSERT_EQUAL_UINT(num_pts, 5);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4 * 5);
}

/*******************
 * clip_degenerate *
 *******************/

/**************************************
 *     -15, 15           15,15        *
 *        y _______________           *
 *         |              |           *
 *         |        _ - v1|           *
 *         |    . -       | \         *
 *      _ ,|- `           |   \       *
 *   v2----|--------------|----v0     *
 *         |              |           *
 *         |______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/

/* clip on a list containing degenerate triangle */
void 
clip_degenerate()
{

    float src[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -15, 0, 0, 15,     /* v0 */
        15, 0, 0, 15,      /* v1 */
        15, 10, 0, 15,     /* degenerate triangle edge case */
        -20, 0, 0, 15      /* v2 */
    };

    float ans[16 * SR_MAX_ATTRIBUTE_COUNT] = {
        -15, 0, 0, 15,
        -15, 0, 0, 15,    /* produces new degenerate triangle */
        15, 0, 0, 15,
        15, 10, 0, 15,
        -15, 1.428572, 0, 15
    };

    float dest[16 * SR_MAX_ATTRIBUTE_COUNT];

    size_t num_pts = 4;
    clip_routine(src, dest, &num_pts, 4, 0, -1);

    TEST_ASSERT_EQUAL_UINT(num_pts, 5);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4 * 5);
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
    RUN_TEST(clip_no_pts);
    RUN_TEST(cull_triangle);
    RUN_TEST(clip_one_pt);
    RUN_TEST(clip_two_pts);
    RUN_TEST(clip_three_pts);
    RUN_TEST(clip_attr);
    RUN_TEST(miss_axes);
    RUN_TEST(clip_pt_on_edge);
    RUN_TEST(clip_degenerate);

    return UNITY_END();
}

