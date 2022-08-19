
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "unity.h"
#include "sr_render.c"
#include "sr_matrix.h"

/*********************************************************************
 *                                                                   *
 *                        shader declarations                        *
 *                                                                   *
 *********************************************************************/

static void
vs_basic(void* uniform, float* in, float* out);

static void
fs_basic(void* uniform, float* in, uint32_t* out);

/*********************************************************************
 *                                                                   *
 *                           unity helpers                           *
 *                                                                   *
 *********************************************************************/

uint32_t g_colors[10 * 10];
float g_depths[10 * 10];
int g_uniform = 1;

struct sr_framebuffer g_fbuf = {
    .width = 10,
    .height = 10,
    .colors = g_colors,
    .depths = g_depths
};

struct sr_pipeline_context g_pipe = {
    .fbuf = &g_fbuf,
    .uniform = &g_uniform,
    .vs = vs_basic,
    .fs = fs_basic,
    .pts_in = NULL,
    .n_pts = 0,
    .n_attr_in = 5,
    .n_attr_out = 5,
    .winding = SR_WINDING_ORDER_CCW
};

void 
setUp()
{
    /* clear framebuffer */
    memset(g_colors, 0, sizeof(uint32_t) * 10 * 10);
    for (int i = 0; i < 10 * 10; i++) {
        g_depths[i] = 100000;
    }
    g_uniform = 0;
}

void 
tearDown()
{
    /* empty */
}

static void
vs_basic(void* uniform, float* in, float* out)
{
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    out[3] = in[3];
    out[4] = in[4];
}

static void
vs_transform(void* uniform, float* in, float* out)
{
    matmul_v(out, (float*)uniform, in);
    out[4] = in[4];
}


static void
fs_basic(void* uniform, float* in, uint32_t* out)
{
    *out = ceil(in[4]);
}

/*********************************************************************
 *                                                                   *
 *                            basic tests                            *
 *                                                                   *
 *********************************************************************/

/*************
 * one_point *
 *************/

/* renders exactly one point */
void 
one_point() 
{
    float pts_in[5] = {0, 0, 0, 1, 1};
    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 1;
    int indices[1] = {0};
    sr_draw_indexed(&g_pipe, indices, 1, SR_PRIMITIVE_TYPE_POINT_LIST);

    uint32_t target_colors[10 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
}

/****************
 * three_points *
 ****************/


/* renders three points */
void 
three_points() 
{
    float pts_in[5 * 3] = {
        0, 0, 0, 3, 1,
        2, 0, 0, 3, 1,
        0, 2, 0, 3, 1
    };

    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 3;

    int indices[3] = {0, 1, 2};
    sr_draw_indexed(&g_pipe, indices, 3, SR_PRIMITIVE_TYPE_POINT_LIST);

    uint32_t target_colors[10 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    for (int i = 0; i < 10 * 10; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%d ", g_colors[i]);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
}


/****************
 * one_triangle *
 ****************/

/* renders exactly one triangle */
void 
one_triangle() 
{

    float pts_in[5 * 3] = {
        0, 0, 0, 3, 1,
        2, 0, 0, 3, 1,
        0, 2, 0, 3, 1
    };

    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 3;

    int indices[3] = {0, 1, 2};
    sr_draw_indexed(&g_pipe, indices, 3, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);

     uint32_t target_colors[10 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    for (int i = 0; i < 10 * 10; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%d ", g_colors[i]);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
}

/*******************
 * three_triangles *
 *******************/

/* renders three triangles */
void 
three_triangles() 
{
    float pts_in[5 * 3 * 3] = {
        0, 0, 0, 10, 1,
        5, 0, 0, 10, 1,
        0, 5, 0, 10, 1,

        0, 6, 0, 10, 2,    /* this triangle too small to render */
        3, 6, 0, 10, 2,
        3, 10, 0, 10, 2,

        -5, -9, 0, 10, 3, /* this is a wild triangle */
        4, 0, 0, 10, 3,
        -9, 4, 0, 10, 3
    };

    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 9;

    int indices[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    sr_draw_indexed(&g_pipe, indices, 9, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);
    
    uint32_t target_colors[10 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    for (int i = 0; i < 10 * 10; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%d ", g_colors[i]);
    }
    printf("\n");


    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
}


/****************
 * triangle_fan *
 ****************/

/* renders three triangles in a fan */
void
triangle_fan() 
{
    float pts_in[5 * 3 * 3] = {
        0, 0, 0, 10, 1,
        5, 0, 0, 10, 1,
        0, 5, 0, 10, 1,
        -2.5, 9, 0, 10, 1,
        -5, -7, 0, 10, 1

    };

    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 5;

    int indices[9] = {0, 1, 2, 0, 2, 3, 0, 3, 4};
    sr_draw_indexed(&g_pipe, indices, 9, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);

    uint32_t target_colors[10 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

        for (int i = 0; i < 10 * 10; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%d ", g_colors[i]);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
}

/*********************************************************************
 *                                                                   *
 *                           perspective                             *
 *                                                                   *
 *********************************************************************/

/***********************
 * triangle_same_depth *
 **********************/

/* triangle has vertices at varying depths */
void
flat_depth() 
{
    float pts_in[5 * 3] = {
        0, -5, 6, 6, 1,
        5, 5, 6, 6, 1,
        -5, 5, 6, 6, 1
    };

    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 3;

    int indices[3] = {0, 1, 2};
    sr_draw_indexed(&g_pipe, indices, 3, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);

     uint32_t target_colors[10 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    for (int i = 0; i < 10 * 10; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%d ", g_colors[i]);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
}


/***********************
 * triangle_same_depth *
 **********************/

/* triangle has vertices at varying depths */
void
near_depth() 
{
    float pts_in[5 * 3] = {
        0, -5, 10, 10, 1,
        5, 5, 6, 6, 1,
        -5, 5, 6, 6, 1
    };

    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 3;

    int indices[3] = {0, 1, 2};
    sr_draw_indexed(&g_pipe, indices, 3, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);

    uint32_t target_colors[10 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    for (int i = 0; i < 10 * 10; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%d ", g_colors[i]);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
}

/***********************
 * triangle_same_depth *
 **********************/

/* triangle has vertices at varying depths */
void
far_depth() 
{
    float pts_in[5 * 3] = {
        0, 5, 10000, 10000, 1,
        5, 5, 6, 6, 1,
        -5, 5, 6, 6, 1
    };

    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 3;

    int indices[3] = {0, 1, 2};
    sr_draw_indexed(&g_pipe, indices, 3, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);
    
    uint32_t target_colors[10 * 10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

      for (int i = 0; i < 10 * 10; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%d ", g_colors[i]);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
}

/*********************************************************************
 *                                                                   *
 *                            vertex shader                          *
 *                                                                   *
 *********************************************************************/

/*********************
 * projection matrix *
 *********************/

/* vertex shader sends points from view space to clip space */
void
projection_matrix()
{
    float fov = 60 * (3.14159 / 180);
    float a = 10 / 10;    /* aspect ratio */
    float n = 1;          /* near plane */
    float f = 100;        /* far plane */
    float ta = tan( fov / 2 );

    float projection_matrix[4 * 4] = {
        1/(ta*a),  0,         0,            0,
        0,         1/ta,      0,            0,
        0,         0,         -(f+n)/(f-n), 2*f*n/(f-n),
        0,         0,        -1,            0
    };

    g_pipe.uniform = projection_matrix;
    g_pipe.vs = vs_transform;

    float pts_in[5 * 3] = {
        11, -20, -20, 1, 2,
        -30, 30, -43, 1, 2,
        -20, 0, -30, 1, 2
    };

    /**
     *   float pts_in[5 * 6] = {
            -3, 0, 12, 1.0, 2,
            0.3, 0.1, 0.5, 1.0, 2,
            -20, 10, 43, 1.0, 2
        };
     * 
     */

    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 3;

    int indices[3] = {0, 1, 2};
    sr_draw_indexed(&g_pipe, indices, 3, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);

    uint32_t target_colors[10 * 10] = {
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 1, 0, 0
    };

    for (int i = 0; i < 10 * 10; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%d ", g_colors[i]);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
}


/*********************
 * projection matrix *
 *********************/

/* vertex shader sends points from view space to clip space */
void
another_projection_test()
{
    float fov = 60 * (3.14159 / 180);
    float a = 10 / 10;    /* aspect ratio */
    float n = 1;          /* near plane */
    float f = 100;        /* far plane */
    float ta = tan( fov / 2 );

    float projection_matrix[4 * 4] = {
        1/(ta*a),  0,         0,            0,
        0,         1/ta,      0,            0,
        0,         0,         -(f+n)/(f-n), 2*f*n/(f-n),
        0,         0,        -1,            0
    };

    g_pipe.uniform = projection_matrix;
    g_pipe.vs = vs_transform;

    float pts_in[5 * 3] = {
        5, 10, -40, 1, 2,
        -10, 10, -41, 1, 2,
        2, -20, -70, 1, 2
    };

    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 3;

    int indices[3] = {0, 1, 2};
    sr_draw_indexed(&g_pipe, indices, 3, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);

    uint32_t target_colors[10 * 10] = {
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 1, 0, 0
    };

    for (int i = 0; i < 10 * 10; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%d ", g_colors[i]);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
}

/***************
 * view_matrix *
 ***************/

/* vertex shader sends points from world space to clip space */
void
view_matrix() 
{
    
}

/*********************
 * mvw matrix *
 *********************/

/* vertex shader sends points from model space to clip space */
void
mvw_matrix() 
{
    
}

/*******************
 * translate_point *
 *******************/

/* output point is a translation of input */
void 
translate_point() 
{

}

/**********************
 * translate_triangle *
 **********************/

/* output triangle is a translation of input */
void 
translate_triangle() 
{

}

/*********************************************************************
 *                                                                   *
 *                             clipping                              *
 *                                                                   *
 *********************************************************************/

/************************
 * clip_three_triangles *
 ************************/

/* renders three triangles */
void 
clip_three_triangles() 
{
    float pts_in[5 * 3 * 3] = { /* all clip space coords */
        25, -3, 58.9, 59, 1,
        4, 5, -100, 65, 1,
        -10, -10, 8, 10, 1,
        
        1.4, -1.3, -0.5, 1.4, 2,
        50, 15, 28, 30, 2,
        0, 45, 28, 30, 2,

        4, 66, 65.12, 65, 3,
        20, 72, 70.6, 70, 3,
        20, 75, 66.2, 66, 3
    };

    g_pipe.pts_in = pts_in;
    g_pipe.n_pts = 9;

    int indices[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    sr_draw_indexed(&g_pipe, indices, 9, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);

    uint32_t target_colors[10 * 10] = {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    };

        for (int i = 0; i < 10 * 10; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("%d ", g_colors[i]);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_UINT32_ARRAY(target_colors, g_colors, 10 * 10);
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
    RUN_TEST(three_triangles);
    RUN_TEST(one_point);
    RUN_TEST(three_points);
    RUN_TEST(one_triangle);
    RUN_TEST(triangle_fan);
    RUN_TEST(flat_depth);
    RUN_TEST(near_depth);
    RUN_TEST(far_depth);
    RUN_TEST(clip_three_triangles);
    RUN_TEST(projection_matrix);
    RUN_TEST(another_projection_test);
    return UNITY_END();
}

