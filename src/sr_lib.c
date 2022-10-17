
#include <stdlib.h>

#include "sr.h"
#include "sr_math.h"
#include "sr_lib.h"

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
 *          global variables that control pipeline state             *
 *                                                                   *
 *********************************************************************/

/* identity matrix */
const struct mat4 identity = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/* model matrix */
struct mat4 model = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/* normal model matrix */
struct mat4 normal_model = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/* camera view matrix */
struct mat4 view = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/* projection matrix */
struct mat4 proj = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/* model view projection matrix */
struct mat4 mvp = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

struct mat4* cur_mat;  /* points to whichever matrix stack is being used */
struct light_slot* cur_light;

struct sr_texture texture = {
    .colors = 0,
    .width = 0,
    .height = 0
};

float cam_pos[3] = {
    0, 0, 0
};

/* framebuffer */
struct sr_framebuffer fbuf = {
    .width = 0,
    .height = 0,
    .colors = 0,
    .depths = 0
};

/* uniform */
struct sr_uniform uniform = {
    .model = &model,
    .normal_model = &normal_model,
    .mvp = &mvp,
    .texture = &texture,
};

/* pipeline state */
struct sr_pipeline pipe = {
    .fbuf = &fbuf,
    .uniform = (void*)(&uniform),
    .vs = 0,
    .fs = 0,
    .pts_in = 0,
    .n_pts = 0,
    .n_attr_in = 0,
    .n_attr_out = 0,
    .winding = SR_WINDING_ORDER_CCW
};

/*********************************************************************
 *                                                                   *
 *                         render interface                          *
 *                                                                   *
 *********************************************************************/

/**************
 * sr_renderl *
 **************/

/* builds the mvp and renders the global state */
extern void
sr_renderl(int* indices, int n_indices, enum sr_primitive prim_type)
{
    /* create mvp */
    mvp = identity;
    matmul(&mvp, &proj);
    matmul(&mvp, &view);
    matmul(&mvp, &model);

    /* create normal model matrix */
    normal_model = model;
    mat3(&normal_model);
    invert(&normal_model);

    /* send down the pipeline */
    sr_render(&pipe, indices, n_indices, prim_type);
}

/*********************************************************************
 *                                                                   *
 *                   pipeline and uniform bindings                   *
 *                                                                   *
 *********************************************************************/

/***************
 * sr_bind_pts *
 ***************/

/* sets points */
extern void
sr_bind_pts(float* pts, int n_pts, int n_attr)
{
    pipe.pts_in = pts;
    pipe.n_pts = n_pts;
    pipe.n_attr_in = n_attr;
}

/***********************
 * sr_bind_framebuffer *
 ***********************/

/* attaches color and depth buffers to global framebuffer */
extern void
sr_bind_framebuffer(int width, int height, uint32_t* colors, float* depths)
{
    fbuf.width = width;
    fbuf.height = height;
    fbuf.colors = colors;
    fbuf.depths = depths;
}

/**************
 * sr_bind_vs *
 **************/

/* sets the vertex shader */
extern void
sr_bind_vs(vs_f vs, int n_attr_out)
{
    pipe.vs = vs;
    pipe.n_attr_out = n_attr_out;
}

/**************
 * sr_bind_fs *
 **************/

/* sets the fragment shader */
extern void
sr_bind_fs(fs_f fs)
{
    pipe.fs = fs;
}

/*******************
 * sr_bind_texture *
 *******************/

/* binds a texture to pipeline */
extern void
sr_bind_texture(uint32_t* colors, int width, int height)
{
    texture.colors = colors;
    texture.width = width;
    texture.height = height;
}

/*****************
 * sr_bind_color *
 *****************/

/* binds a base color to uniform */
extern void
sr_bind_base_color(float r, float g, float b)
{
    uniform.base_color[0] = r;
    uniform.base_color[1] = g;
    uniform.base_color[2] = b;
}

/*********************************************************************
 *                                                                   *
 *                           light slot                              *
 *                                                                   *
 *********************************************************************/

/************
 * sr_light *
 ************/

/* binds a light to pipeline */
extern void 
sr_light(enum sr_light slot, enum sr_light_attr attr, float* data)
{
    /* split index */
    int idx = 0;
    switch(slot) {
        case SR_LIGHT_1:
            idx = 0;
            break;
        case SR_LIGHT_2:
            idx = 1;
            break;
        case SR_LIGHT_3:
            idx = 2;
            break;
        case SR_LIGHT_4:
            idx = 3;
            break;    
        case SR_LIGHT_5:
            idx = 4;
            break;    
        case SR_LIGHT_6:
            idx = 5;
            break;    
        case SR_LIGHT_7:
            idx = 6;
            break;    
        case SR_LIGHT_8:
            idx = 7;
            break;    
    }

    /* set one-hot state */
    uniform.light_state |= 1 << idx;

    /* split attribute data */
    switch(attr) {
        case SR_AMBIENT:
            uniform.lights[idx].ambient = *data;
            break;
        case SR_DIFFUSE:
            uniform.lights[idx].diffuse = *data;
            break;
        case SR_SPECULAR:
            uniform.lights[idx].specular = *data;
            break;
        case SR_POSITION:
            memcpy(uniform.lights[idx].pos, data, 3 * sizeof(float));
            break;
        case SR_COLOR:
            memcpy(uniform.lights[idx].color, data, 3 * sizeof(float));
            break;
        case SR_SPOT_DIRECTION:
            memcpy(uniform.lights[idx].spot_dir, data, 3 * sizeof(float));
            break;
        case SR_SPOT_EXPONENT:
            uniform.lights[idx].spot_exp = *data;
            break;
        case SR_SPOT_CUTOFF:
            uniform.lights[idx].spot_cutoff = *data;
            break;
        case SR_CONSTANT_ATTENUATION:
            uniform.lights[idx].attn_const = *data;
            break;
        case SR_LINEAR_ATTENUATION:
            uniform.lights[idx].attn_lin = *data;
            break;
        case SR_QUADRATIC_ATTENUATION:
            uniform.lights[idx].attn_quad = *data;
            break;
    }
}

/*********************************************************************
 *                                                                   *
 *                      matrix stack operations                      *
 *                                                                   *
 *********************************************************************/

/******************
 * sr_matrix_mode *
 ******************/

/* sets current matrix stack */
extern void
sr_matrix_mode(enum sr_matrix_mode mode)
{
    switch (mode) {
        case SR_MODEL_MATRIX:
            cur_mat = &model;
            break;
        case SR_VIEW_MATRIX:
            cur_mat = &view;
            break;
        case SR_PROJECTION_MATRIX:
            cur_mat = &proj;
            break;
        case SR_MVP_MATRIX:
            cur_mat = &mvp;
            break;
    }
}

/***************
 * sr_dump_mat *
 ***************/

/* dumps contents of current matrix to given buffer in row major order */
extern void
sr_dump_matrix(float* dest)
{
    dest[0] = cur_mat->e00;
    dest[1] = cur_mat->e01;
    dest[2] = cur_mat->e02;
    dest[3] = cur_mat->e03;
    dest[4] = cur_mat->e10;
    dest[5] = cur_mat->e11;
    dest[6] = cur_mat->e12;
    dest[7] = cur_mat->e13;
    dest[8] = cur_mat->e20;
    dest[9] = cur_mat->e21;
    dest[10] = cur_mat->e22;
    dest[11] = cur_mat->e23;
    dest[12] = cur_mat->e30;
    dest[13] = cur_mat->e31;
    dest[14] = cur_mat->e32;
    dest[15] = cur_mat->e33;
}

/******************
 * sr_load_matrix *
 ******************/

/* loads the entries in a 16 length row major float array to current matrix */
extern void
sr_load_matrix(float* src)
{
    cur_mat->e00 = src[0];
    cur_mat->e01 = src[1];
    cur_mat->e02 = src[2];
    cur_mat->e03 = src[3];
    cur_mat->e10 = src[4];
    cur_mat->e11 = src[5];
    cur_mat->e12 = src[6];
    cur_mat->e13 = src[7];
    cur_mat->e20 = src[8];
    cur_mat->e21 = src[9];
    cur_mat->e22 = src[10];
    cur_mat->e23 = src[11];
    cur_mat->e30 = src[12];
    cur_mat->e31 = src[13];
    cur_mat->e32 = src[14];
    cur_mat->e33 = src[15];
}

/********************
 * sr_load_identity *
 ********************/

/* sets current matrix to the identity */
extern void
sr_load_identity()
{
   *cur_mat = identity;
}

/*********************************************************************
 *                                                                   *
 *                              model                                *
 *                                                                   *
 *********************************************************************/

/****************
 * sr_translate *
 ****************/

/* pushes an affine transformation with translation of x, y, z */
extern void
sr_translate(float x, float y, float z)
{
    struct mat4 t = {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };

    matmul(cur_mat, &t);
}

/***************
 * sr_rotate_x *
 ***************/

/* pushes a matrix rotating about the x axis by t radians */
extern void
sr_rotate_x(float t)
{
    float c = cos(t);
    float s = sin(t);

    struct mat4 x = {
        1,  0,  0,  0,
        0,  c,  -s, 0,
        0,  s,  c,  0,
        0,  0,  0,  1
    };

    matmul(cur_mat, &x);
}

/***************
 * sr_rotate_y *
 ***************/
float camera_pos[3] = {
    0, 0, 0
};

/* pushes a matrix rotating about the y axis by t radians */
extern void
sr_rotate_y(float t)
{
    float c = cos(t);
    float s = sin(t);

    struct mat4 y = {
        c,  0,  s,  0,
        0,  1,  0,  0,
        -s, 0,  c,  0,
        0,  0,  0,  1
    };

    matmul(cur_mat, &y);
}

/***************
 * sr_rotate_z *
 ***************/

/* pushes a matrix rotating about the z axis by t radians */
extern void
sr_rotate_z(float t)
{
    float c = cos(t);
    float s = sin(t);
    
    struct mat4 z = {
        c,  -s, 0,  0,
        s,  c,  0,  0,
        0,  0,  1,  0,
        0,  0,  0,  1
    };

    matmul(cur_mat, &z);
}

/************
 * sr_scale *
 ************/

/* pushes a scale matrix by sx, sy, sz */
extern void
sr_scale(float sx, float sy, float sz)
{
    struct mat4 s = {
        sx, 0,  0,  0,
        0,  sy, 0,  0,
        0,  0,  sz, 0,
        0,  0,  0,  1
    };

    matmul(cur_mat, &s);
}

/*********************************************************************
 *                                                                   *
 *                              view                                 *
 *                                                                   *
 *********************************************************************/

/**************
 * sr_look_at *
 **************/

/* 
 * constructs a view matrix from three vectors :
 * the eye vector, camera position in world space
 * the target vector, the position of view target in world space
 * the up vector, pointing to whats generally above the camera
 */
extern void
sr_look_at(float ex, float ey, float ez, 
           float lx, float ly, float lz, 
           float ux, float uy, float uz)
{
    /* eye vector */
    float eye[3] = {
        ex, ey, ez
    };

    /* look vector */
    float look[3] = {
        lx, ly, lz
    };

    /* up vector */
    float up[3] = {
        ux, uy, uz
    };

    /* backward vector, w */
    float w[3];
    sub_v(w, eye, look);
    normalize(w);

    /* side vector, u */
    float u[3];
    cross(u, up, w);   
    normalize(u);

    /* new up vector, v */
    float v[3];
    float up_proj_w[3]; /* up projected onto w */
    scale_v(up_proj_w, w, dot(up, w));
    sub_v(v, up, up_proj_w);
    normalize(v);
    
    struct mat4 m = {
        u[0], u[1], u[2], 0,
        v[0], v[1], v[2], 0,
        w[0], w[1], w[2], 0,
        0,    0,    0,    1
    };

    uniform.cam_pos[0] = ex;
    uniform.cam_pos[1] = ey;
    uniform.cam_pos[2] = ez;

    matmul(cur_mat, &m);
    sr_translate(-ex, -ey, -ez);
}

/*********************************************************************
 *                                                                   *
 *                            projection                             *
 *                                                                   *
 *********************************************************************/

/******************
 * sr_perspective *
 ******************/

/* pushes a perspective matrix specified by fov */
extern void
sr_perspective(float fovy, float aspect, float near, float far)
{
    float f = 1 / (tan(fovy / 2));
    float e22 = (far + near) / (near - far);
    float e23 =  (2 * far * near) / (near - far);
    float a = aspect;

    struct mat4 p = {
        f/a, 0,   0,    0,
        0,   f,   0,    0,
        0,   0,   e22,  e23,
        0,   0,   -1,    0
    };

    matmul(cur_mat, &p);
}

/**************
 * sr_frustum *
 **************/

/* pushes a projection matrix based on frustum */
extern void
sr_frustum(float left, float right, float bottom, 
           float top, float near, float far)
{
    float e00 = (2 * near) / (right - left);
    float e11 = (2 * near) / (top - bottom);
    float e02 = (right + left) / (right - left);
    float e12 = (top + bottom) / (top - bottom);
    float e22 = -(far + near) / (near - far);
    float e23 = -(2 * far * near) / (near - far);

    struct mat4 p = {
        e00, 0,   e02,  0,
        0,   e11, e12,  0,
        0,   0,   e22,  e23,
        0,   0,   -1,    0
    };

    matmul(cur_mat, &p);
}