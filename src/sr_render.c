
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
ndc(float* pts, size_t num_pts, size_t num_attr);

/*********************************************************************
 *                                                                   *
 *                         public definition                         *
 *                                                                   *
 *********************************************************************/

/******************
 * sr_draw_arrays *
 ******************/

/**
 * entry point of the sr pipeline, 
 * refines indexed vertex data to be sent to rasterizer
 */
void
sr_draw_arrays(struct sr_pipeline_context* pipe,
               size_t* indices, 
               size_t num_indices,
               size_t prim_type)
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

    for (int i = 0; i < pipe->num_pts; i++) {
        pipe->vs(pipe->uniform, 
                 pipe->pts_in + i * num_attr_in, 
                 pts_out + i * num_attr_out);
    }

    for (int i = 0; i <= num_prims; i += prim_sz) {

        /* primitive assembly */
        for (int j = 0; j < prim_sz; j++) {
            memset(tmp_p + j * num_attr_out, 
                   pts_out + indices[i + j], 
                   num_attr_out * sizeof(float));
        }

        /* clipping */
        size_t num_pts_clip = prim_sz;
        int in_frustum = 1;
        if (prim_sz == SR_PRIMITIVE_SIZE_TRIANGLE) {
            in_frustum = clip(tmp_p, tmp_p, &num_pts_clip);
        }

         /* perspective divide */
        ndc(tmp_p, prim_sz, num_attr_out);

        /* send to raster stage */
        switch (prim_sz) {
            case SR_PRIMITIVE_SIZE_POINT:
                draw_pt(&rast, tmp_p);
                break;
            case SR_PRIMITIVE_SIZE_LINE:
                draw_ln(&rast, tmp_p, tmp_p + num_attr_out);
                break;
            case SR_PRIMITIVE_SIZE_TRIANGLE:
                int bfc = backface_cull(pipe->winding_order
                                        tmp_p, 
                                        tmp_p + num_attr_out, 
                                        tmp_p + 2 * num_attr_out)
                if (bfc && in_frustum) {
                    draw_tr(&rast,
                            tmp_p, 
                            tmp_p + num_attr_out,
                            tmp_p + 2 * num_attr_out);
                }
                break;
        }
    }
    free(pts_out);
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

/* moves coordinates from clip space to normalized device space */
static void
ndc(float* pts, size_t num_pts, size_t num_attr)
{
    for (int i = 0; i < num_pts; i += num_attr) {

    }
}
