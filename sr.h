
#ifndef SR_H
#define SR_H

#include <stdint.h>
#include <stddef.h>

#define SR_MAX_ATTRIBUTE_COUNT 32
#define SR_MAX_LIGHT_COUNT 8

#define SR_CLIP_LEFT_PLANE 1 << 0
#define SR_CLIP_BOTTOM_PLANE 1 << 1
#define SR_CLIP_NEAR_PLANE 1 << 2
#define SR_CLIP_RIGHT_PLANE 1 << 3
#define SR_CLIP_TOP_PLANE 1 << 4

#define SR_WINDING_ORDER_CCW 1
#define SR_WINDING_ORDER_CW -1

enum sr_primitive {
    SR_POINT_LIST,
    SR_LINE_LIST,
    SR_LINE_STRIP,
    SR_TRIANGLE_LIST,
    SR_TRIANGLE_STRIP
};

enum sr_matrix_mode {
    SR_MODEL_MATRIX,
    SR_VIEW_MATRIX,
    SR_PROJECTION_MATRIX,
    SR_MVP_MATRIX
};

enum sr_light {
    SR_LIGHT_1,
    SR_LIGHT_2,
    SR_LIGHT_3,
    SR_LIGHT_4,
    SR_LIGHT_5,
    SR_LIGHT_6,
    SR_LIGHT_7,
    SR_LIGHT_8,
};

enum sr_light_attr {
    SR_TYPE,
    SR_POSITION,
    SR_COLOR,
    SR_AMBIENT,
    SR_DIFFUSE,
    SR_SPECULAR,
    SR_BLEND,
    SR_SHININESS,
    SR_DIRECTION,
    SR_SPOT_ANGLE,
    SR_SPOT_PENUMBRA,
    SR_CONSTANT_ATTENUATION,
    SR_LINEAR_ATTENUATION,
    SR_QUADRATIC_ATTENUATION
};

enum sr_light_type {
    SR_DIRECTIONAL,
    SR_POINT,
    SR_SPOT
};

/*********************************************************************
 *                                                                   *
 *                     shader function pointers                      *
 *                                                                   *
 *********************************************************************/

typedef void (*vs_f)(float* out, float* in, void* uniform);
typedef void (*fs_f)(uint32_t* out, float* in, void* uniform);

/*********************************************************************
 *                                                                   *
 *                      public data structures                       *
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


/******************
 * sr_framebuffer *
 ******************/

/* interface to whatever writes to the screen */
struct sr_framebuffer {
    uint32_t* colors;
    float* depths;           
    int width; 
    int height;    
};

/***************
 * sr_pipeline *
 ***************/

/**
 * stores all data required to pre-allocate
 * memory required to render an indexed list of
 * arbitrary length
 */
struct sr_pipeline {
    struct sr_framebuffer* fbuf;
    void* uniform;        
    vs_f vs;
    fs_f fs;

    float* pts_in;
    int n_pts;
    int n_attr_in;
    int n_attr_out;

    int winding;
};

/*********************************************************************
 *                                                                   *
 *                             file input                            *
 *                                                                   *
 *********************************************************************/


/*********************************************************************
 *                                                                   *
 *                           struct helpers                          *
 *                                                                   *
 *********************************************************************/


/*********************************************************************
 *                                                                   *
 *                           render pipeline                         *
 *                                                                   *
 *********************************************************************/

/* render interface */

extern void
sr_bind_pts(float* pts, int n_pts, int n_attr);

extern void
sr_bind_framebuffer(int width, int height, uint32_t* colors, float* depths);

extern void
sr_bind_vs(vs_f vs, int n_attr_out);

/* WARNING can't use matrix stack, default shaders, or lights if called */
extern void
sr_bind_uniform(void* uniform);

/* restores default uniform if sr_bind_uniform was called */
extern void
sr_restore_uniform();

extern void
sr_bind_fs(fs_f fs);

extern void
sr_bind_texture(uint32_t* colors, int width, int height);

extern void
sr_bind_base_color(float r, float g, float b);

extern void
sr_renderl(int* indices, int n_indices, enum sr_primitive prim_type);

extern void
sr_render(struct sr_pipeline* pipe, int* indices, 
          int n_indices, enum sr_primitive prim_type);

/*********************************************************************
 *                                                                   *
 *                         light interface                           *
 *                                                                   *
 *********************************************************************/

extern void
sr_light(enum sr_light slot, enum sr_light_attr attr, float* data);

extern void 
sr_glight(enum sr_light_attr attr, float* data);

extern void 
sr_light_type(enum sr_light slot, enum sr_light_type type);

extern void
sr_light_enable(enum sr_light slot);

extern void
sr_light_disable(enum sr_light slot);

extern void
sr_material(enum sr_light_attr attr, float* data);

/*********************************************************************
 *                                                                   *
 *                      matrix stack interface                       *
 *                                                                   *
 *********************************************************************/

extern void
sr_matrix_mode(enum sr_matrix_mode mode);

extern void
sr_dump_matrix(float* dest);

extern void
sr_load_matrix(float* src);

extern void
sr_load_identity();

extern void
sr_perspective(float fovy, float aspect, float near, float far);

extern void
sr_frustum(float left, float right, float bottom, 
           float top, float near, float far);

extern void
sr_translate(float x, float y, float z);

extern void
sr_rotate_x(float t);

extern void
sr_rotate_y(float t);

extern void
sr_rotate_z(float t);

extern void
sr_scale(float sx, float sy, float sz);

extern void
sr_look_at(float ex, float ey, float ez, 
           float cx, float cy, float cz, 
           float ux, float uy, float uz);

/*********************************************************************
 *                                                                   *
 *                     prebuilt shader bindings                      *
 *                                                                   *
 *********************************************************************/

extern void 
sr_bind_color_vs();

extern void
sr_bind_color_fs();

extern void
sr_bind_texture_vs();

extern void
sr_bind_texture_fs();

extern void
sr_bind_std_vs();

extern void
sr_bind_phong_fs();

#endif /* SR_H */
