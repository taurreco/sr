
#include "sr.h"
#include "sr_math.h"

/**
 * sr_lib.h
 * --------
 * an easy interface with the sr pipeline for
 * your average use case
 * 
 * provides a uniform with model view projection matrix,
 * and matrix stack-like operations to build it
 * 
 */

#ifndef SR_LIB_H
#define SR_LIB_H

/*********************************************************************
 *                                                                   *
 *                              structs                              *
 *                                                                   *
 *********************************************************************/

struct texture {
    uint32_t* colors;
    int width;
    int height;
};

struct sr_point_light {
    float* pos;
    float* color;
    float diffuse;
    float ambient;
    float const_attn;
    float lin_attn;
    float quad_attn;
};

struct sr_uniform {
    struct mat4* model;
    struct mat4* mvp;
    struct texture* texture;
    struct sr_point_light* light;
    float* base_color;
    float* cam_pos;
};

#endif  /* SR_LIB_H */

