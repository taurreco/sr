
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_raster.h"

/*********************************************************************
 *                                                                   *
 *                           declarations                            *
 *                                                                   *
 *********************************************************************/

static void fs(void* uniform, uint32_t* color_p, point_t* pt_p);

/*********************************************************************
 *                                                                   *
 *                        setup raster data                          *
 *                                                                   *
 *********************************************************************/

uint32_t g_cbuf[5 * 5];
float g_zbuf[5 * 5];

struct sr_fbuf g_fbuf = {5, 5, g_cbuf, g_zbuf};
struct raster_data g_rdata = {NULL, &g_fbuf, fs, 4};

/*********************************************************************
 *                                                                   *
 *                   fragment shader definition                      *
 *                                                                   *
 *********************************************************************/

/* sets color to 1 */
static void 
fs(void* uniform, uint32_t* color_p, point_t* pt_p)
{
    (*color_p) = 1;
}

/*********************************************************************
 *                                                                   *
 *                          unity helpers                            *
 *                                                                   *
 *********************************************************************/

void 
setUp() 
{
    /* clear framebuffer */
    memset(g_cbuf, 0, sizeof(uint32_t) * 5 * 5);
    for (int i = 0; i < 5 * 5; i++) {
        g_zbuf[i] = 1000;
    }
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

/*************
 * one_point *
 *************/

/* draw a point to the first slot in the frame buffer. */

void 
one_point() 
{
    float pt[4] = {0, 0, 0, 0};
    draw_pt(&g_rdata, pt);

    uint32_t target_cbuf[5 * 5] = {
        1, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 5 * 5);
}

/****************
 * three_points *
 ****************/

/* draw three points to the frame buffer in sequence */

void 
three_points() 
{
    float pt1[4] = {1, 1, 0, 0};
    draw_pt(&g_rdata, pt1);

    uint32_t target_cbuf_first[5 * 5] = {
        0, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf_first, g_rdata.fbuf->cbuf, 5 * 5);

    float pt2[4] = {0, 2, 0, 0};
    draw_pt(&g_rdata, pt2);

    uint32_t target_cbuf_second[5 * 5] = {
        0, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        1, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf_second, g_rdata.fbuf->cbuf, 5 * 5);
    
    float pt3[4] = {1, 2, 0, 0};
    draw_pt(&g_rdata, pt3);

    uint32_t target_cbuf_third[5 * 5] = {
        0, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        1, 1, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf_third, g_rdata.fbuf->cbuf, 5 * 5);
}

/*********************************************************************
 *                                                                   *
 *                          depth buffer                             *
 *                                                                   *
 *********************************************************************/

/****************
 * depth_simple *
 ****************/

/* draw one point with different depth */

void 
depth_simple() 
{
float pt[4] = {0, 0, 10, 0};
    draw_pt(&g_rdata, pt);

    float target_zbuf[5 * 5] = {
        10, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(target_zbuf, g_rdata.fbuf->zbuf, 5 * 5);
}

/*****************
 * depth_overlap *
 ****************/

/* two points attempt to render at the same position with different depths */

void 
depth_overlap() 
{
    float pt1[4] = {0, 0, 2, 0};
    draw_pt(&g_rdata, pt1);

    float pt2[4] = {0, 0, 4, 0};
    draw_pt(&g_rdata, pt2);

    float target_zbuf[5 * 5] = {
        2, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(target_zbuf, g_rdata.fbuf->zbuf, 5 * 5);
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
    RUN_TEST(one_point);
    RUN_TEST(three_points);
    RUN_TEST(depth_simple);
    RUN_TEST(depth_overlap);
    return UNITY_END();
}

