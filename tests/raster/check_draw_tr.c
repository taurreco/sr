
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "unity.h"
#include "sr_raster.h"

/*********************************************************************
 *                                                                   *
 *                           declarations                            *
 *                                                                   *
 *********************************************************************/

static void fs_attr(void* uniform, uint32_t* color_p, point_t* pt_p);
static void fs_color(void* uniform, uint32_t* color_p, point_t* pt_p);

/*********************************************************************
 *                                                                   *
 *                        setup raster data                          *
 *                                                                   *
 *********************************************************************/

uint32_t g_color;
uint32_t g_cbuf[6 * 10];
float g_zbuf[6 * 10];

struct sr_fbuf g_fbuf = {10, 6, g_cbuf, g_zbuf};
struct raster_data g_rdata = {&g_color, &g_fbuf, fs_color, 4};

/*********************************************************************
 *                                                                   *
 *                   fragment shader definitions                     *
 *                                                                   *
 *********************************************************************/

/* whatever is in the fourth attribute slot is the 'color' */

static void
fs_attr(void* uniform, uint32_t* color_p, point_t* pt_p) 
{
    (*color_p) = (*pt_p)[3];
}

/* the uniform data becomes the 'color' */

static void 
fs_color(void* uniform, uint32_t* color_p, point_t* pt_p)
{
    (*color_p) = *((uint32_t*)(uniform));
}

/*********************************************************************
 *                                                                   *
 *                           unity helpers                           *
 *                                                                   *
 *********************************************************************/
void 
setUp() 
{
    /* clear framebuffer */
    memset(g_cbuf, 0, sizeof(uint32_t) * 6 * 10);
    for (int i = 0; i < 6 * 10; i++) {
        g_zbuf[i] = 1000;
    }
    /* set default color to 1 */
    g_color = 1;
    g_rdata.fs = fs_color;
}

void 
tearDown() 
{
    /* empty */
}

/*********************************************************************
 *                                                                   *
 *                          trivial case                             *
 *                                                                   *
 *********************************************************************/

/******************
 * basic_triangle *
 ******************/

/* small easy triangle to draw */

void 
basic_triangle() 
{
    /* triangle points */

    float tr[3 * 4] = {
        2.5, 1.15, 1, 0,         /* v0 */
        1.2, 2.73, 1, 0,         /* v1 */
        4.0, 4.0, 1, 0            /* v2 */
    };

    draw_tr(&g_rdata, tr, tr + 4, tr + 8);


    uint32_t target_cbuf[6 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/*********************************************************************
 *                                                                   *
 *                           fill rules                              *
 *                                                                   *
 *********************************************************************/

/******************
 * fill_too_small *
 ******************/

/* input points too close to each other and no pixel centers are touched */

void 
fill_too_small() 
{

    float tr[3 * 4] = {
        2.2, 1.15, 1, 0,         /* v0 */
        1.2, 2.15, 1, 0,         /* v1 */
        2.1, 2.1, 1, 0             /* v2 */
    };

    draw_tr(&g_rdata, tr, tr + 4, tr + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/*******************
 * fill_on_centers *
 *******************/

/* input points each on adjacent pixel centers */

void 
fill_on_centers() 
{

    float tr[3 * 4] = {
        2.5, 1.5, 1, 0,         /* v0 */
        1.5, 2.5, 1, 0,         /* v1 */
        2.5, 2.5, 1, 0            /* v2 */
    };

    draw_tr(&g_rdata, tr, tr + 4, tr + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/************************
 * fill_two_right_edges *
 ************************/

/* input triangle has two right edges which lie on pixel centers */

void 
fill_two_right_edges() 
{
    float tr[3 * 4] = {
        2.5, 1.5, 1, 0,         /* v0 */
        2.5, 3.5, 1, 0,         /* v1 */
        3.5, 2.5, 1, 0            /* v2 */
    };

    draw_tr(&g_rdata, tr, tr + 4, tr + 8);

    /* none of the pixels touched by right edges are rendered */

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/*************************
 * fill_joined_triangles *
 *************************/

/* 
 * two triangles with a shared edge, but the bottom
 * triangle only touches pixels with its right edge 
 */

void 
fill_joined_triangles() 
{
    float tr1[3 * 4] = {
        4.0, 0.0, 1.0, 0.0,         /* v0 */
        2.5, 1.5, 1.0, 0.0,         /* v1 */
        3.5, 2.5, 1.0, 0.0            /* v2 */
    };

    draw_tr(&g_rdata, tr1, tr1 + 4, tr1 + 8);

    g_color = 2;

    /* new triangle sharing an edge with the above */

    float tr2[3 * 4] = {
        3.5, 4.5, 1.0, 0.0,         /* v0 */
        2.5, 1.5, 1.0, 0.0,         /* v1 */
        3.5, 2.5, 1.0, 0.0            /* v2 */
    };

    draw_tr(&g_rdata, tr2, tr2 + 4, tr2 + 8);

    /* only the top triangle is rendered */

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/********************
 * fill_unit_quad *
 ********************/

/* a quad made by two triangles, less of the right triangle renders */

void 
fill_unit_quad() 
{
    float tr1[3 * 4] = {
        1.5, 1.5, 1.0, 0.0,         /* v0 */
        3.5, 1.5, 1.0, 0.0,         /* v1 */
        1.5, 3.5, 1.0, 0.0            /* v2 */
    };

    draw_tr(&g_rdata, tr1, tr1 + 4, tr1 + 8);

    g_color = 2;

    float tr2[3 * 4] = {
        3.5, 3.5, 1.0, 0.0,         /* v0 */
        3.5, 1.5, 1.0, 0.0,         /* v1 */
        1.5, 3.5, 1.0, 0.0            /* v2 */
    };

    draw_tr(&g_rdata, tr2, tr2 + 4, tr2 + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/*************************
 * fill_two_shared_edges *
 *************************/

/* three triangles where one shares an edge with the next */

void 
fill_two_shared_edges() 
{
    float tr1[3 * 4] = {
        0.0, 3.0, 1.0, 0.0,         /* v0 */
        6.0, 1.0, 1.0, 0.0,         /* v1 */
        4.0, 3.0, 1.0, 0.0            /* v2 */
    };

    draw_tr(&g_rdata, tr1, tr1 + 4, tr1 + 8);

    g_color = 2;

    float tr2[3 * 4] = {
        7.0, 4.0, 1.0, 0.0,         /* v0 */
        6.0, 1.0, 1.0, 0.0,         /* v1 */
        4.0, 3.0, 1.0, 0.0            /* v2 */
    };

    draw_tr(&g_rdata, tr2, tr2 + 4, tr2 + 8);

    g_color = 3;

    float tr3[3 * 4] = {
        7.0, 4.0, 1.0, 0.0,         /* v0 */
        6.0, 1.0, 1.0, 0.0,         /* v1 */
        8.5, 2.5, 1.0, 0.0            /* v2 */
    };

    draw_tr(&g_rdata, tr3, tr3 + 4, tr3 + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 2, 3, 0, 0, 0,
        0, 1, 1, 1, 2, 2, 3, 3, 0, 0,
        0, 0, 0, 0, 0, 2, 2, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };



    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/*********************************************************************
 *                                                                   *
 *                    interpolated attributes                        *
 *                                                                   *
 *********************************************************************/

/*********************
 * attr_same_weights *
 *********************/

/* interpolated values are all the same */

void
attr_same_weights()
{
    g_rdata.fs = fs_attr;

    float tr[3 * 4] = {
        0.0, 3.0, 0.0, 300.0,           /* v0 */
        5.0, 3.0, 0.0, 300.0,           /* v1 */
        2.5, 0.0, 0.0, 300.0            /* v2 */
    };

    draw_tr(&g_rdata, tr, tr + 4, tr + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 300, 0, 0, 0, 0, 0, 0, 0,
        0, 300, 300, 300, 0, 0, 0, 0, 0, 0,
        300, 300, 300, 300, 300, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/*********************
 * attr_similar_weights *
 *********************/

/* interpolated values between similar weights */

void
attr_similar_weights()
{
    g_rdata.fs = fs_attr;

    float tr[3 * 4] = {
        0.0, 3.0, 0.0, 300.0,           /* v0 */
        5.0, 3.0, 0.0, 310.0,           /* v1 */
        2.5, 0.0, 0.0, 280.0            /* v2 */
    };

    draw_tr(&g_rdata, tr, tr + 4, tr + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 284, 0, 0, 0, 0, 0, 0, 0,
        0, 290, 292, 294, 0, 0, 0, 0, 0, 0,
        296, 298, 300, 302, 304, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}


/***********************
 * attr_varied_weights *
 ***********************/

/* tests attribute interpolation among different edges of weights */

void
attr_varied_weights()
{
    g_rdata.fs = fs_attr;

    float tr[3 * 4] = {
        0.0, 3.0, 0.0, 400.0,           /* v0 */
        5.0, 3.0, 0.0, 900.0,           /* v1 */
        2.5, 0.0, 0.0, 300.0            /* v2 */
    };

    draw_tr(&g_rdata, tr, tr + 4, tr + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 358, 0, 0, 0, 0, 0, 0, 0,
        0, 375, 475, 575, 0, 0, 0, 0, 0, 0,
        391, 491, 591, 691, 791, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}


/***********************
 * attr_negative_weights *
 ***********************/

/* tests attribute interpolation on an isocoles triangle */

void
attr_negative_weights()
{
    g_rdata.fs = fs_attr;

    float tr[3 * 4] = {
        0.0, 3.0, 0.0, 451.0,           /* v0 */
        5.0, 3.0, 0.0, -901.0,           /* v1 */
        2.5, 0.0, 0.0, 638.0            /* v2 */
    };

    draw_tr(&g_rdata, tr, tr + 4, tr + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 494, 0, 0, 0, 0, 0, 0, 0,
        0, 476, 206, -63, 0, 0, 0, 0, 0, 0,
        459, 189, -81, -351, -621, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/*************************
 * attr_negative_weights *
 *************************/

/* tests attribute interpolation on a scalene triangle */

void
attr_scalene()
{
     g_rdata.fs = fs_attr;

    float tr[3 * 4] = {
        3.0, 0.0, 0.0, 400.0,           /* v0 */
        1.0, 2.0, 0.0, 900.0,           /* v1 */
        5.0, 5.0, 0.0, 300.0            /* v2 */
    };

    draw_tr(&g_rdata, tr, tr + 4, tr + 8);
    
    uint32_t target_cbuf[6 * 10] = {
        0, 0, 525, 0, 0, 0, 0, 0, 0, 0,
        0, 775, 582, 389, 0, 0, 0, 0, 0, 0,
        0, 0, 639, 446, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 503, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 367, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/*********************************************************************
 *                                                                   *
 *                overlap between triangles (depth)                  *
 *                                                                   *
 *********************************************************************/

/**************
 * depth_same *
 **************/

/* 
 * both triangles overlap and are at the same depth, 
 * the first to render will take the cake
 */

void
depth_same()
{
    float tr1[3 * 4] = {
        0.0, 3.0, 0.0, 400.0,           /* v0 */
        5.0, 3.0, 0.0, 900.0,           /* v1 */
        2.5, 0.0, 0.0, 300.0            /* v2 */
    };

    draw_tr(&g_rdata, tr1, tr1 + 4, tr1 + 8);

    g_color = 2;
    
    float tr2[3 * 4] = {
        3.0, 0.0, 0.0, 400.0,           /* v0 */
        1.0, 2.0, 0.0, 900.0,           /* v1 */
        5.0, 5.0, 0.0, 300.0            /* v2 */
    };

    draw_tr(&g_rdata, tr2, tr2 + 4, tr2 + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/**************************
 * depth_complete_overlap *
 **************************/

/* one triangle partially overlaps the other */

void
depth_complete_overlap()
{
    float tr1[3 * 4] = {
        0.0, 3.0, 10.0, 400.0,           /* v0 */
        5.0, 3.0, 10.0, 900.0,           /* v1 */
        2.5, 0.0, 10.0, 300.0            /* v2 */
    };

    draw_tr(&g_rdata, tr1, tr1 + 4, tr1 + 8);

    g_color = 2;
    
    float tr2[3 * 4] = {
        3.0, 0.0, 0.0, 400.0,           /* v0 */
        1.0, 2.0, 0.0, 900.0,           /* v1 */
        5.0, 5.0, 0.0, 300.0            /* v2 */
    };

    draw_tr(&g_rdata, tr2, tr2 + 4, tr2 + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 2, 0, 0, 0, 0, 0, 0, 0,
        0, 2, 2, 2, 0, 0, 0, 0, 0, 0,
        1, 1, 2, 2, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
}

/****************
 * depth_varied *
 ****************/

/* 
 * the bottom of the first triangle is overlapped by the second, 
 * but the top of the second is overlapped by the first
 */

void
depth_varied()
{
    float tr1[3 * 4] = {
        0.0, 3.0, 100.0, 400.0,           /* v0 */
        5.0, 3.0, 100.0, 900.0,           /* v1 */
        2.5, 0.0, 100.0, 300.0            /* v2 */
    };

    draw_tr(&g_rdata, tr1, tr1 + 4, tr1 + 8);

    g_color = 2;
    
    float tr2[3 * 4] = {
        3.0, 0.0, 130.0, 400.0,           /* v0 */
        1.0, 2.0, 130.0, 900.0,           /* v1 */
        5.0, 5.0, 0.0, 300.0            /* v2 */
    };

    draw_tr(&g_rdata, tr2, tr2 + 4, tr2 + 8);

    uint32_t target_cbuf[6 * 10] = {
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 2, 0, 0, 0, 0, 0, 0,
        1, 1, 2, 2, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_cbuf, g_rdata.fbuf->cbuf, 6 * 10);
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
    /* base case */
    RUN_TEST(basic_triangle);
    /* fill rules */
    RUN_TEST(fill_too_small);
    RUN_TEST(fill_on_centers);
    RUN_TEST(fill_two_right_edges);
    RUN_TEST(fill_joined_triangles);
    RUN_TEST(fill_unit_quad);
    RUN_TEST(fill_two_shared_edges);
    /* attribute interpolation */
    RUN_TEST(attr_same_weights);
    RUN_TEST(attr_similar_weights);
    RUN_TEST(attr_varied_weights);
    RUN_TEST(attr_negative_weights);
    RUN_TEST(attr_scalene);
    /* overlap between triangles */
    RUN_TEST(depth_same);
    RUN_TEST(depth_complete_overlap);
    RUN_TEST(depth_varied);
    return UNITY_END();
}
