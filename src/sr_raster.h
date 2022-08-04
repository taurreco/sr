#include "sr.h"
#include "sr_config.h"

#ifndef SR_RASTER_H
#define SR_RASTER_H

#if UNIT_TEST
#define u_static  
#else
#define u_static static
#endif

/*********************************************************************
 *                                                                   *
 *                     public data structures                        *
 *                                                                   *
 *********************************************************************/

struct raster_context {
    struct sr_framebuffer* fbuf;
    void* uniform;
    fs_f fs;
    size_t n_attr;
    int wind_dir;
};

/*********************************************************************
 *                                                                   *
 *                      public declarations                          *
 *                                                                   *
 *********************************************************************/

void 
draw_pt(struct raster_context* rast, float* pt);

void 
draw_ln(struct raster_context* rast, float* v0, float* v1);

void 
draw_tr(struct raster_context* rast, float* v0, float* v1, float* v2);

/*********************************************************************
 *                                                                   *
 *                      private declarations                         *
 *                                                                   *
 *********************************************************************/

#if UNIT_TEST

/***************
 * struct edge *
 ***************/

/* holds the steps sizes / data for barycentric weight for an edge */
struct edge {
    float step_x, step_y;    /* steps to increment to get the det at new pt */
    int is_tl;               /* tracks if the edge is top left */
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
float 
edge_init(struct edge* edge, float* v0, float* v1, float* pt);

void 
bbox_init(struct bbox* bbox, float* v0, float* v1, float* v2);

/* utility */
int
is_tl(float* v0, float* v1);

#endif


#endif /* SR_RASTER_H */