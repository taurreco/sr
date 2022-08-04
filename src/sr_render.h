
#include "sr_config.h"
#include "sr.h"
#include "sr_raster.h"

#ifndef SR_RENDER_H
#define SR_RENDER_H

#if UNIT_TEST
#define u_static  
#else
#define u_static static
#endif

/*********************************************************************
 *                                                                   *
 *                       public declarations                         *
 *                                                                   *
 *********************************************************************/

/* implements the public draw_indexed function */
void
sr_draw_indexed(struct sr_pipeline_context* pipe, size_t* indices, 
                size_t n_indices, size_t prim_type);

/*********************************************************************
 *                                                                   *
 *                      private declarations                         *
 *                                                                   *
 *********************************************************************/

#if UNIT_TEST

void
split_primitive(size_t prim_type, size_t* prim_sz);

int
winding_order(int dir, float* v0, float* v1, float* v2);

void 
screen_space(struct sr_framebuffer* fbuf, float* pt);

#endif


#endif /* SR_RENDER_H */
