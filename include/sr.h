
#ifndef SR_H
#define SR_H

#include <stdint.h>
#include <stddef.h>

#define SR_MAX_ATTRIBUTE_COUNT 32                   

#define SR_PRIMITIVE_SIZE_POINT 1
#define SR_PRIMITIVE_SIZE_LINE 2
#define SR_PRIMITIVE_SIZE_TRIANGLE 3

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

enum sr_color_type {
    SR_RGBA32,
    SR_RGBA16,
    SR_RBA5
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


/*************************
 * struct sr_framebuffer *
 *************************/

/* interface to whatever writes to the screen */
struct sr_framebuffer {
    int width, height;    /* width, height */
    uint32_t* colors;        /* colors */
    float* depths;           /* depths */
};

/******************************
 * struct sr_pipeline_context *
 ******************************/

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

extern void
sr_bind_fs(fs_f fs);

extern void
sr_bind_texture(uint32_t* texture, int width, int height);

extern void
sr_bind_point_light(float* pos, float* color);

extern void
sr_bind_base_color(float* color);

extern void
sr_renderl(int* indices, int n_indices, enum sr_primitive prim_type);

extern void
sr_render(struct sr_pipeline* pipe, int* indices, 
          int n_indices, enum sr_primitive prim_type);

/*********************************************************************
 *                                                                   *
 *                             file input                            *
 *                                                                   *
 *********************************************************************/

extern int
sr_load_obj(float** pts_p, int** indices_p, int* n_pts_p, 
            int* n_attr_p, int* n_indices_p, char* file);

extern int
sr_load_tga(uint32_t** texture_p, int* width_p, 
            int* height_p, char* file);

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
sr_rotate_x(float r);

extern void
sr_rotate_y(float p);

extern void
sr_rotate_z(float y);

extern void
sr_scale(float sx, float sy, float sz);

extern void
sr_look_at(float ex, float ey, float ez, 
           float cx, float cy, float cz, 
           float ux, float uy, float uz);

/*********************************************************************
 *                                                                   *
 *                              shaders                              *
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
sr_bind_phong_vs();

extern void
sr_bind_phong_fs();

#endif /* SR_H */
