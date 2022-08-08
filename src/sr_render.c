
#include <stdlib.h>
#include <string.h>

#include "sr.h"
#include "sr_raster.h"
#include "sr_render.h"
#include "sr_clip.h"

/*********************************************************************
 *                                                                   *
 *                       private declarations                        *
 *                                                                   *
 *********************************************************************/

/* utility */
u_static void
draw_prim(struct raster_context* rast, float* pts, 
          size_t n_pts, size_t prim_type);

u_static void 
split_prim(size_t prim_type, size_t* prim_sz);

u_static int
winding_order(int dir, float* v0, float* v1, float* v2);

u_static void
screen_space(struct sr_framebuffer* fbuf, float* pt);

/*********************************************************************
 *                                                                   *
 *                         public definition                         *
 *                                                                   *
 *********************************************************************/

/*******************
 * sr_draw_indexed *
 *******************/

/**
 * entry point of the sr pipeline, 
 * refines indexed vertex data to be sent to rasterizer
 */
void
sr_draw_indexed(struct sr_pipeline_context* pipe, size_t* idxs, 
                size_t n_idxs, size_t prim_type)
{
    /* setup variables */
    
    struct raster_context rast = {
        .fbuf = pipe->fbuf, 
        .uniform = pipe->uniform, 
        .fs = pipe->fs, 
        .n_attr = pipe->n_attr_out,
        .wind_dir = pipe->wind_dir
    };

    size_t prim_sz;
    split_prim(prim_type, &prim_sz);
    size_t n_prims = n_idxs / prim_sz;

    /* vertex processing */

    size_t pts_out_sz = pipe->n_pts * pipe->n_attr_out;
    
    float* pts_out = malloc(pts_out_sz * sizeof(float));
    uint8_t* clip_flags = malloc(pipe->n_pts * sizeof(uint8_t));

    for (size_t i = 0; i < pipe->n_pts; i++) {    /* per point */

        /* vertex shader pass */
        pipe->vs(pipe->uniform, 
                 pipe->pts_in + i * pipe->n_attr_in, 
                 pts_out + i * pipe->n_attr_out);

        /* grab clip flags while still hot */
        clip_test(pts_out + i * pipe->n_attr_out, clip_flags + i);
    }

    float tmp[16 * SR_MAX_ATTRIBUTE_COUNT]; /* holds current face */

    for (size_t i = 0; i < n_prims * prim_sz; i += prim_sz) {

        /* primitive assembly */

        uint8_t clip_and, clip_or = 0;

        for (size_t j = 0; j < prim_sz; j++) {
            /* fill buffer with primitive data */
            memcpy(tmp + j * pipe->n_attr_out,
                   pts_out + idxs[i + j] * pipe->n_attr_out, 
                   pipe->n_attr_out * sizeof(float));

            /* accumulate point clip flags */
            clip_and &= clip_flags[i + j];
            clip_or |= clip_flags[i + j];
        }

        /* clipping */

        if (clip_and != 0)  /* outside frustum */
            break;

        size_t prim_clip_sz = prim_sz;
        if (clip_or != 0)     /* if not intersect frustum */
            clip_poly(tmp, &prim_clip_sz, pipe->n_attr_out, clip_or);

        /* perspective divide */
        for (size_t j = 0; j < prim_clip_sz; j++)
            screen_space(pipe->fbuf, tmp + j * pipe->n_attr_out);
        
        draw_prim(&rast, tmp, prim_clip_sz, prim_type);
    }
    free(pts_out);
    free(clip_flags);
}

/*********************************************************************
 *                                                                   *
 *                       private definitions                         *
 *                                                                   *
 *********************************************************************/

/*************
 * draw_prim *
 *************/

/* matches the correct drawing routine with the primitive type */
u_static void 
draw_prim(struct raster_context* rast, float* pts, 
          size_t n_pts, size_t prim_type)
{
        switch (prim_type) {
            case SR_PRIMITIVE_TYPE_POINT_LIST:    /* point list */
                for (size_t i = 0; i < n_pts; i++) {
                    draw_pt(rast, pts + i * rast->n_attr);
                }
                break;
            case SR_PRIMITIVE_TYPE_LINE_LIST:
            case SR_PRIMITIVE_TYPE_LINE_STRIP:    /* line list */
                for (size_t i = 1; i < n_pts; i++) {
                   /* draw_ln(rast, tmp_p, tmp_p + i * n_attr_out); */
                }
                break;
            case SR_PRIMITIVE_TYPE_TRIANGLE_LIST:
            case SR_PRIMITIVE_TYPE_TRIANGLE_STRIP:    /* triangle fan */
                {
                    float* v0 = pts;
                    float* v1 = pts + 1 * rast->n_attr;
                    for (size_t i = 2; i < n_pts; i++) {
                        float* v2 = pts + i * rast->n_attr;
                        if (winding_order(rast->wind_dir, v0, v1, v2))
                            draw_tr(rast, v0, v1, v2);
                        v1 = v2;
                    }
                }
                break;
        }
}

/**************
 * split_prim *
 **************/

/* fills relevant traversal data about a primitive type */
u_static void
split_prim(size_t prim_type, size_t* prim_sz)
{
    switch (prim_type) {
        case SR_PRIMITIVE_TYPE_POINT_LIST:
            *prim_sz = SR_PRIMITIVE_SIZE_POINT;
            break;
        case SR_PRIMITIVE_TYPE_LINE_LIST:
        case SR_PRIMITIVE_TYPE_LINE_STRIP:
            *prim_sz = SR_PRIMITIVE_SIZE_LINE;
            break;
        case SR_PRIMITIVE_TYPE_TRIANGLE_LIST:
        case SR_PRIMITIVE_TYPE_TRIANGLE_STRIP:
            *prim_sz = SR_PRIMITIVE_SIZE_TRIANGLE;
            break;
    }
}

/*****************
 * winding_order *
 *****************/

/* false for triangles against the winding order */
u_static int
winding_order(int dir, float* v0, float* v1, float* v2)
{
    float e01 = (v1[0] - v0[0]) * (v1[1] + v0[1]);
    float e12 = (v2[0] - v1[0]) * (v2[1] + v1[1]);
    float e20 = (v0[0] - v2[0]) * (v0[1] + v2[1]);

    return (e01 + e12 + e20) * dir > 0;  /* same sign */
}

/****************
 * screen_space *
 ****************/

/* moves coordinates from clip space to screen space */
u_static void
screen_space(struct sr_framebuffer* fbuf, float* pt)
{
    /* to ndc space */

    pt[3] = 1 / pt[3];
    pt[0] *= pt[3];
    pt[1] *= pt[3];
    pt[2] *= pt[3]; /* should equal 1 now */

    /* to screen space */
    pt[0] = (fbuf->width / 2) * (pt[0] + 1);
    pt[1] = (fbuf->height / 2) * (1 - pt[1]);
    pt[2] = (pt[2] + 1) / 2;
}

