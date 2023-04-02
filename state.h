
#ifndef SR_LIB_H
#define SR_LIB_H

#include "sr.h"
#include "mat.h"

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
    uint8_t type;
    float pos[3];
    float color[4];

    float dir[3];
    float spot_angle;
    float spot_penumbra;

    float attn_const;
    float attn_lin;
    float attn_quad;
};

/************
 * material *
 ************/

struct material {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float blend;
    float shininess;
};

/**************
 * sr_uniform *
 **************/

/* the uniform variables for the fixed lib shaders */
struct sr_uniform {

    /* geometry */
    struct mat4* model;
    struct mat4* normal_transform;
    struct mat4* mvp;
    float cam_pos[3];

    /* material */
    int has_texture;
    struct material* material;
    struct sr_texture* texture;

    /* light */
    uint8_t light_state;
    struct light* lights;
    float ka;
    float kd;
    float ks;
};

#endif  /* SR_LIB_H */

