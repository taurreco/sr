
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "sr_raster.h"

/*********************************************************************
 *                                                                   *
 *                      private declarations                         *
 *                                                                   *
 *********************************************************************/

#if !(UNIT_TEST)

/***************
 * struct edge *
 ***************/

/* holds the steps sizes / data for barycentric weight for an edge */
struct edge {
    float step_x, step_y;       /* steps to increment det by to get the det at new pt */
    int is_tl;                  /* tracks if the edge is top left */
};

/***************
 * struct bbox *
 ***************/

/* holds the upper and lower bounds of the triangle */
struct bbox {
    float min_x, min_y;
    float max_x, max_y;
};

/* constructors */
u_static float 
edge_init(struct edge* edge, float* v0, float* v1, float* pt);

u_static void 
bbox_init(struct bbox* bbox, float* v0, float* v1, float* v2);

/* utility */
u_static int 
is_tl(float* v0, float* v1);

#endif

/*********************************************************************
 *                                                                   *
 *                        public definitions                         *
 *                                                                   *
 *********************************************************************/

/***********
 * draw_pt *
 ***********/

/* render ptoint to framebuffer */
void 
draw_pt(struct raster_context* rast, float* pt)
{
    uint32_t color = 0; /* color dest */
    rast->fs(rast->uniform, pt, &color);
    size_t fb_idx = floorf(pt[1]) * rast->fbuf->width + floorf(pt[0]);
    
    if (pt[2] < rast->fbuf->depths[fb_idx]) {  /* deptth buffer */
        rast->fbuf->colors[fb_idx] = color;
        rast->fbuf->depths[fb_idx] = pt[2];
    }
}

/***********
 * draw_tr *
 ***********/

/* rasterize a triangle to framebuffer */
void 
draw_tr(struct raster_context* rast, float* v0, float* v1, float* v2)
{   
    /* find bounding box */

    struct bbox bbox; 
    bbox_init(&bbox, v0, v1, v2);

    /* store current point */

    float pt[SR_MAX_ATTRIBUTE_COUNT];
    pt[0] = bbox.min_x;
    pt[1] = bbox.min_y;

    /* grab edge information */

    struct edge e12, e20, e01;

    float w0_row = edge_init(&e12, v1, v2, pt);
    float w1_row = edge_init(&e20, v2, v0, pt);
    float w2_row = edge_init(&e01, v0, v1, pt);

    /* rasterize */

    for (pt[1] = bbox.min_y; pt[1] <= bbox.max_y; pt[1]++) {

        float w0 = w0_row;
        float w1 = w1_row;
        float w2 = w2_row;

        for (pt[0] = bbox.min_x; pt[0] <= bbox.max_x; pt[0]++) {

            float b0 = (w0 == 0) && !e12.is_tl ? -1 : 0;
            float b1 = (w1 == 0) && !e20.is_tl ? -1 : 0;
            float b2 = (w2 == 0) && !e01.is_tl ? -1 : 0;

            if ( (w0 + b0 >= 0) && (w1 + b1 >= 0) && (w2 + b2 >= 0) ) {

                /* normalize weights */

                float area = w0 + w1 + w2;
                
                float w0_nm = w0 / area;    /* nm suffix for 'NorMailzed' */
                float w1_nm = w1 / area;
                float w2_nm = w2 / area;

                /* interpolate attributes */

                for (int i = 2; i < (int)rast->num_attr; i++) {
                    pt[i] = w0_nm * v0[i] + w1_nm * v1[i] + w2_nm * v2[i];
                }

                draw_pt(rast, pt);
            }

            w0 += e12.step_x;
            w1 += e20.step_x;
            w2 += e01.step_x;
        }

        w0_row += e12.step_y;
        w1_row += e20.step_y;
        w2_row += e01.step_y;
    }
}

/*********************************************************************
 *                                                                   *
 *                       private constructors                        *
 *                                                                   *
 *********************************************************************/

/*************
 * edge_init *
 *************/

/* make an edge struct instance, return the initial determinant */
u_static float 
edge_init(struct edge* edge, float* v0, float* v1, float* pt)
{
    edge->is_tl = is_tl(v0, v1);

    float A = v1[1] - v0[1];
    float B = v0[0] - v1[0];
    float C = v0[1] * v1[0] - v0[0] * v1[1];

    /* step sizes */
    edge->step_x = A;
    edge->step_y = B;

    /* edge function output at origin */
    return A * pt[0] + B * pt[1] + C;
}

/*************
 * bbox_init *
 *************/

/* define a pixel-aligned bounding box for triangle rasterization */
u_static void
bbox_init(struct bbox* bbox, float* v0, float* v1, float* v2)
{    
    /* naiive values */

    bbox->min_x = fmin(v0[0], fmin(v1[0], v2[0]));
    bbox->min_y = fmin(v0[1], fmin(v1[1], v2[1]));
    bbox->max_x = fmax(v0[0], fmax(v1[0], v2[0]));
    bbox->max_y = fmax(v0[1], fmax(v1[1], v2[1]));

    /* align to pixel centers */

    bbox->min_x = floorf(bbox->min_x) + 0.5;
    bbox->min_y = floorf(bbox->min_y) + 0.5;
    bbox->max_x = floorf(bbox->max_x) + 0.5;
    bbox->max_y = floorf(bbox->max_y) + 0.5;
}

/*********************************************************************
 *                                                                   *
 *                      private utility helpers                      *
 *                                                                   *
 *********************************************************************/

/*********
 * is_tl *
 *********/

/* determines whether an edge is top or left */
u_static int 
is_tl(float* v0, float* v1) 
{
    int is_top = (v0[1] == v1[1]) && (v0[0] > v1[0]); 
    int is_left = v0[1] < v1[1];
    return is_top | is_left;
}