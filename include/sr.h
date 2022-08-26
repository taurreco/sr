
#ifndef SR_H
#define SR_H

#include <stdint.h>
#include <stddef.h>

#define SR_MAX_ATTRIBUTE_COUNT 32                   

#define SR_PRIMITIVE_TYPE_POINT_LIST 1
#define SR_PRIMITIVE_TYPE_LINE_LIST 2
#define SR_PRIMITIVE_TYPE_LINE_STRIP 3
#define SR_PRIMITIVE_TYPE_TRIANGLE_LIST 4
#define SR_PRIMITIVE_TYPE_TRIANGLE_STRIP 5

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

/*********************************************************************
 *                                                                   *
 *                     shader function pointers                      *
 *                                                                   *
 *********************************************************************/

typedef void (*vs_f)(void* uniform, float* in, float* out);
typedef void (*fs_f)(void* uniform, float* in, uint32_t* out);

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
/*
extern void
sr_viewport(int width, int height); */

extern void
sr_fixed_draw(int* indices, int n_indices, uint8_t prim_type);

extern void
sr_draw_indexed(struct sr_pipeline* pipe, int* indices, 
                int n_indices, uint8_t prim_type);

/*********************************************************************
 *                                                                   *
 *                             obj loading                           *
 *                                                                   *
 *********************************************************************/

extern void
sr_load_obj(float** pts_p, int** indices_p, int* n_pts_p, 
            int* n_attr_p, int* n_indices_p, char* file);

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
sr_bind_vs_test();

extern void
sr_bind_fs_test();

#endif /* SR_H */
