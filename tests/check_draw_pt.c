
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_rast.c"

/*********************************************************************
 *                                                                   *
 *                           declarations                            *
 *                                                                   *
 *********************************************************************/

static void fs(uint32_t* color_p, float* pt void* uniform,);

/*********************************************************************
 *                                                                   *
 *                        setup raster data                          *
 *                                                                   *
 *********************************************************************/

uint32_t g_colors[5 * 5];
float g_depths[5 * 5];

struct sr_framebuffer g_fbuf = {
    .width = 5, 
    .height = 5, 
    .colors = g_colors, 
    .depths = g_depths
};

struct raster_context g_rast = {
    .fbuf = &g_fbuf, 
    .uniform = NULL,
    .fs = (fs_f)fs, 
    .n_attr = 4
};

/*********************************************************************
 *                                                                   *
 *                   fragment shader definition                      *
 *                                                                   *
 *********************************************************************/

/* sets color to 1 */
static void 
fs(uint32_t* color_p, float* pt, void* uniform)
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
    memset(g_colors, 0, sizeof(uint32_t) * 5 * 5);
    for (int i = 0; i < 5 * 5; i++) {
        g_depths[i] = 1000;
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
    draw_pt(&g_rast, pt);

    uint32_t target_colors[5 * 5] = {
        1, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_rast.fbuf->colors, 5 * 5);
}

/****************
 * three_points *
 ****************/

/* draw three points to the frame buffer in sequence */

void 
three_points() 
{
    float pt1[4] = {1, 1, 0, 0};
    draw_pt(&g_rast, pt1);

    uint32_t target_colors_first[5 * 5] = {
        0, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors_first, g_rast.fbuf->colors, 5 * 5);

    float pt2[4] = {0, 2, 0, 0};
    draw_pt(&g_rast, pt2);

    uint32_t target_colors_second[5 * 5] = {
        0, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        1, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors_second, g_rast.fbuf->colors, 5 * 5);
    
    float pt3[4] = {1, 2, 0, 0};
    draw_pt(&g_rast, pt3);

    uint32_t target_colors_third[5 * 5] = {
        0, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        1, 1, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors_third, g_rast.fbuf->colors, 5 * 5);
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
    draw_pt(&g_rast, pt);

    float target_depths[5 * 5] = {
        10, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(target_depths, g_rast.fbuf->depths, 5 * 5);
}

/*****************
 * depth_overlap *
 ****************/

/* two points attempt to render at the same position with different depths */

void 
depth_overlap() 
{
    float pt1[4] = {0, 0, 2, 0};
    draw_pt(&g_rast, pt1);

    float pt2[4] = {0, 0, 4, 0};
    draw_pt(&g_rast, pt2);

    float target_depths[5 * 5] = {
        2, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000,
        1000, 1000, 1000, 1000, 1000
    };

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(target_depths, g_rast.fbuf->depths, 5 * 5);
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

