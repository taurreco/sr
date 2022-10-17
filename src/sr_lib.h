
#include "sr.h"
#include "sr_math.h"

/**
 * sr_lib.h
 * --------
 * an easy interface with a fixed subset of the sr pipeline 
 * for your average use case
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

/*********
 * light *
 *********/

/* holds light data */
struct light {
    float pos[3];
    float color[3];

    float ambient;
    float diffuse;
    float specular;

    float spot_dir[3];
    float spot_exp;
    float spot_cutoff;

    float attn_const;
    float attn_lin;
    float attn_quad; 
};

/**************
 * sr_uniform *
 **************/

/* the uniform variables for the fixed lib shaders */
struct sr_uniform {
    struct mat4* model;
    struct mat4* normal_model;
    struct mat4* mvp;
    struct sr_texture* texture;
    
    float base_color[3];
    float cam_pos[3];

    uint8_t light_state;
    struct light lights[SR_MAX_LIGHT_COUNT];
};

#endif  /* SR_LIB_H */

