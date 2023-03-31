
#ifndef SR_RASTER_H
#define SR_RASTER_H

#include "sr.h"

/**
 * sr_rast.h
 * --------
 * rasterizes triangles, lines, and points
 * 
 */

/*********************************************************************
 *                                                                   *
 *                     public data structures                        *
 *                                                                   *
 *********************************************************************/

struct raster_context {
    struct sr_framebuffer* fbuf;
    void* uniform;
    fs_f fs;
    int n_attr;
    int winding;
};

/*********************************************************************
 *                                                                   *
 *                      public declarations                          *
 *                                                                   *
 *********************************************************************/

extern void 
draw_pt(struct raster_context* rast, float* pt);

extern void 
draw_ln(struct raster_context* rast, float* v0, float* v1);

extern void 
draw_tr(struct raster_context* rast, float* v0, float* v1, float* v2);

#endif /* SR_RASTER_H */
