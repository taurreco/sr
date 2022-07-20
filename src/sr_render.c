
#include "sr.h"
#include "sr_raster.h"
#include "sr_render.h"

/*********************************************************************
 *                                                                   *
 *                       private declarations                        *
 *                                                                   *
 *********************************************************************/

/* utility */
static void 
split_primitive(size_t prim_type, size_t* prim_sz);

static void 
ndc(float* pt);

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
sr_draw_indexed(struct sr_pipeline_context* pipe, size_t* indices, 
                size_t num_indices, size_t prim_type)
{
    /* setup variables */
    
    size_t prim_sz;
    split_primitve(prim_type, &prim_sz);
    size_t num_prims = num_indices / prim_sz;
    size_t num_attr_in = pipe->num_attr_in;
    size_t num_attr_out = pipe->num_attr_out;

    float tmp[16 * SR_MAX_ATTRIBUTE_COUNT];
    float* tmp_p = (float*)tmp;

    struct raster_context rast = {
        .fbuf = pipe->fbuf, 
        .uniform = pipe->uniform, 
        .fs = pipe->fs, 
        .num_attr = pipe->num_attr_out
    };

    /* vertex processing */

    size_t pts_out_sz = pipe->num_pts * num_attr_out;

    float* pts_out = (float*)malloc(pts_out_sz * sizeof(float));
    uint8_t* clip_flags = (uint8_t*)malloc(pipe->num_pts  * sizeof(uint8_t));

    for (int i = 0; i < pipe->num_pts; i++) {    /* per point */

        /* vertex shader pass */
        pipe->vs(pipe->uniform, 
                 pipe->pts_in + i * num_attr_in, 
                 pts_out + i * num_attr_out);

        /* grab clip flags while still hot */
        clip_test(pts_out + i * num_attr_out, clip_flags + i);
    }

    for (int i = 0; i <= num_prims; i += prim_sz) {    /* per face */

        /* primitive assembly */

        uint8_t clip_and = 0;
        uint8_t clip_or = 0;

        for (int j = 0; j < prim_sz; j++) {

            /* fill buffer with primitive data */
            memset(tmp_p + j * num_attr_out, 
                   pts_out + indices[i + j], 
                   num_attr_out * sizeof(float));

            /* accumulate point clip flags */
            clip_and &= clip_flags[i + j];
            clip_or |= clip_flags[i + j];
        }

        /* clipping */

        if (clip_and != 0)    /* outside frustum */
            break;

        size_t clip_pts = prim_sz;
        if (clip_or != 0)     /* if not intersect frustum */
            clip_poly(tmp_p, &clip_pts, num_attr_out, clip_or);

        /* perspective divide */
        for (int j = 0; j < clip_pts; j++)
            screen_space(pipe->fbuf, tmp_p + j * num_attr_out);

        /* traverse thru a fan of clipped points and draw primitives */
        switch (prim_sz) {
            case 1:    /* point list */
                for (int j = 0; i < clip_pts; j++) {
                    draw_pt(&rast, tmp_p + i);
                }
                break;
            case 2:    /* line strip */
                for (int j = 1; i < clip_pts; j++) {
                    draw_ln(&rast, tmp_p, tmp_p + i);
                }
                break;
            case 3:    /* triangle strip */
                for (int j = 1; i < clip_pts; j += 2) {
                    if (backface_cull(pipe->winding_order, tmp_p))
                        draw_tr(&rast, tmp_p, tmp_p + i, tmp_p + i + 1);
                }
                break;
        }
    }
    free(pts_out);
    free(clip_flags);
}

/*********************************************************************
 *                                                                   *
 *                       private definitions                         *
 *                                                                   *
 *********************************************************************/

/*******************
 * split_primitive *
 *******************/

/* fills relevant traversal data about a primitive type */
static void
split_primitive(size_t prim_type, size_t* prim_sz)
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

/*******
 * ndc *
 *******/

/* moves coordinates from clip space to screen space */
static void
screen_space(struct sr_framebuffer* fbuf, float* pt)
{
    /* to ndc space */
    pt[0] /= pt[3];
    pt[1] /= pt[3];
    pt[2] /= pt[3];
    pt[3] = 1 / pt[3];  /* for perspective correct interpolation */

    /* to screen space */
    pt[0] = fbuf->width * (pt[0] + 1)/2;
    pt[1] = fbuf->height * -(pt[0] + 1)/2;
    pt[2] = (pt[2] + 1) / 2;
}