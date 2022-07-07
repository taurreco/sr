
#include "sr.h"
#include "sr_raster.h"
#include "sr_render.h"

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
    parse_primitve(prim_type, &prim_sz);
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
        if (prim_sz == SR_PRIMITIVE_SIZE_TRIANGLE) {
            clip(tmp_p, tmp_p, &num_pts_clip);
        }

        /* per clipped prim */
        for (int j = 0; j < num_prims; j += prim_sz) {
            float* prim = tmp_p + j * num_attr_out;

            /* perspective divide */
            ndc(prim, prim_sz);

            /* send to raster stage */
            switch (prim_sz) {
                case SR_PRIMITIVE_SIZE_POINT:
                    draw_pt(&rast, prim);
                    break;
                case SR_PRIMITIVE_SIZE_LINE:
                    draw_ln(&rast, prim, prim + num_attr_out);
                    break;
                case SR_PRIMITIVE_SIZE_TRIANGLE:
                    if (backface_cull(prim)) {
                        draw_tr(&rast, 
                                prim, 
                                prim + num_attr_out, 
                                prim + 2 * num_attr_out);
                    }
                    break;
            }
        }
    }
    free(pts_out);
}