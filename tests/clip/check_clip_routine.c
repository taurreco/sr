#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_clip.h"

/**************************************
 *     -15, 15           15,15        *
 *        y _______________           *
 *         |              |           *
 *         |              |           *
 *         |              |           *
 *         |              |           *
 *         |              |           *
 *         |              |           *
 *         |______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/

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
 * clip_neg_x *
 **************/

/* v2 is the only vertex outside the negative x clip plane */

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

void 
clip_neg_x()
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

    int inside = clip_routine(src, dest, &num_pts, 4, 0, -1);

    TEST_ASSERT_EQUAL_UINT(num_pts, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4 * 4);
    TEST_ASSERT_TRUE(inside);
}



/*******************
 * clip_pt_on_edge *
 *******************/

/**
 * will produce a degenerate triangle when a point 
 * outside the box connects to a point exactly on the box 
 */

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

    int inside = clip_routine(src, dest, &num_pts, 4, 0, 1);

    TEST_ASSERT_EQUAL_UINT(num_pts, 5);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4 * 5);
    TEST_ASSERT_TRUE(inside);
}


/* num attr, axis, box size, controlled for */

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


/**************************************
 *     -15, 15           15,15        *
 *        y _______________           *
 *         |              |           *
 *         |              | ___v1     *
 *         |        ___---|     |     *
 *         |      v2--__  |     |     *
 *         |            --|-___  |    *
 *         |              |     v0    *
 *         |______________| x         *
 *     -15,-15          15,-15        *
 *                                    *
 **************************************/



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


/*********************************************************************
 *                                                                   *
 *                              main                                 *
 *                                                                   *
 *********************************************************************/

int
main() 
{
    UNITY_BEGIN();
    RUN_TEST(clip_neg_x);
    RUN_TEST(clip_pt_on_edge);
    return UNITY_END();
}

