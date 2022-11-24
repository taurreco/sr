
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
static const struct mat4 identity = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/* model matrix */
static struct mat4 model = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/* normal transform matrix */
static struct mat4 normal_transform = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/* camera view matrix */
static struct mat4 view = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/* projection matrix */
static struct mat4 proj = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/* model view projection matrix */
static struct mat4 mvp = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

static struct light g_lights[SR_MAX_LIGHT_COUNT];

static struct material g_material;

static struct mat4* cur_mat;  /* points to whichever matrix stack is being used */

static struct sr_texture g_texture = {
    .colors = 0,
    .width = 0,
    .height = 0
};

/* framebuffer */
static struct sr_framebuffer g_fbuf = {
    .width = 0,
    .height = 0,
    .colors = 0,
    .depths = 0
};

/* uniform */
static struct sr_uniform g_uniform = {
    .model = &model,
    .normal_transform = &normal_transform,
    .mvp = &mvp,
    .has_texture = 0,
    .material = &g_material,
    .texture = &g_texture,
    .lights = g_lights,
    .ka = 1,
    .kd = 1,
    .ks = 1
};

/* pipeline state */
static struct sr_pipeline g_pipe = {
    .fbuf = &g_fbuf,
    .uniform = (void*)(&g_uniform),
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

    /* create normal transform matrix */
    normal_transform = model;
    upper_3x3(&normal_transform);
    transpose(&normal_transform);
    invert(&normal_transform);

    /* camera position */
    float origin[4] = {0, 0, 0, 1};
    float tmp[4];
    struct mat4 view_inverse = view;
    invert(&view_inverse);
    vec4_matmul(tmp, &view_inverse, origin);
    memcpy(g_uniform.cam_pos, tmp, 3 * sizeof(float));

    /* send down the pipeline */
    sr_render(&g_pipe, indices, n_indices, prim_type);
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
    g_pipe.pts_in = pts;
    g_pipe.n_pts = n_pts;
    g_pipe.n_attr_in = n_attr;
}

/***********************
 * sr_bind_framebuffer *
 ***********************/

/* attaches color and depth buffers to global framebuffer */
extern void
sr_bind_framebuffer(int width, int height, uint32_t* colors, float* depths)
{
    g_fbuf.width = width;
    g_fbuf.height = height;
    g_fbuf.colors = colors;
    g_fbuf.depths = depths;
}

/*******************
 * sr_bind_uniform *
 *******************/

/* loads a custom uniform */
extern void
sr_bind_uniform(void* uniform)
{
    g_pipe.uniform = uniform;
}

/**********************
 * sr_restore_uniform *
 **********************/

/* loads default uniform */
extern void
sr_restore_uniform()
{
    g_pipe.uniform = &g_uniform;
}

/**************
 * sr_bind_vs *
 **************/

/* sets the vertex shader */
extern void
sr_bind_vs(vs_f vs, int n_attr_out)
{
    g_pipe.vs = vs;
    g_pipe.n_attr_out = n_attr_out;
}

/**************
 * sr_bind_fs *
 **************/

/* sets the fragment shader */
extern void
sr_bind_fs(fs_f fs)
{
    g_pipe.fs = fs;
}

/*******************
 * sr_bind_texture *
 *******************/

/* binds a texture to pipeline */
extern void
sr_bind_texture(uint32_t* colors, int width, int height)
{
    g_uniform.has_texture = 1;
    g_texture.colors = colors;
    g_texture.width = width;
    g_texture.height = height;
}

/*********************************************************************
 *                                                                   *
 *                           light slot                              *
 *                                                                   *
 *********************************************************************/

/***************
 * split_light *
 ***************/

static int
split_light(enum sr_light slot) {
        switch(slot) {
        case SR_LIGHT_1:
            return 0;
        case SR_LIGHT_2:
            return 1;
        case SR_LIGHT_3:
            return 2;
        case SR_LIGHT_4:
            return 3;
        case SR_LIGHT_5:
            return 4;
        case SR_LIGHT_6:
            return 5;
        case SR_LIGHT_7:
            return 6;
        case SR_LIGHT_8:
            return 7;
        default:
            return -1;
    }
    
}

/************
 * sr_light *
 ************/

/* binds a light to pipeline */
extern void 
sr_light(enum sr_light slot, enum sr_light_attr attr, float* data)
{
    /* split index */
    int idx = split_light(slot);

    /* split attribute data */
    switch(attr) {
        case SR_POSITION:
            memcpy(g_lights[idx].pos, data, 3 * sizeof(float));
            break;
        case SR_DIRECTION:
            memcpy(g_lights[idx].dir, data, 3 * sizeof(float));
            break;
        case SR_COLOR:
            memcpy(g_lights[idx].color, data, 4 * sizeof(float));
            break;
        case SR_SPOT_ANGLE:
            g_lights[idx].spot_angle = *data;
            break;
        case SR_SPOT_PENUMBRA:
            g_lights[idx].spot_penumbra = *data;
            break;
        case SR_CONSTANT_ATTENUATION:
            g_lights[idx].attn_const = *data;
            break;
        case SR_LINEAR_ATTENUATION:
            g_lights[idx].attn_lin = *data;
            break;
        case SR_QUADRATIC_ATTENUATION:
            g_lights[idx].attn_quad = *data;
            break;
        default:
            return;
    }
}

/*************
 * sr_glight *
 *************/

/* binds global light data to uniform */
extern void 
sr_glight(enum sr_light_attr attr, float* data)
{
    /* split attribute data */
    switch(attr) {
        case SR_AMBIENT:
            g_uniform.ka = *data;
            break;
        case SR_DIFFUSE:
            g_uniform.kd = *data;
            break;
        case SR_SPECULAR:
            g_uniform.ks = *data;
            break;
        default:
            return;
    }
}

/*****************
 * sr_light_type *
 *****************/

/* binds light type to slot */
extern void 
sr_light_type(enum sr_light slot, enum sr_light_type type)
{
    int idx = split_light(slot);
    switch (type) {
        case SR_DIRECTIONAL:
            g_lights[idx].type = 1 << 0;
            break;
        case SR_POINT:
            g_lights[idx].type = 1 << 1;
            break;
        case SR_SPOT:
            g_lights[idx].type = 1 << 2;
            break;
    }
}


/*******************
 * sr_light_enable *
 *******************/

/* enables light at specified slot */
extern void 
sr_light_enable(enum sr_light slot)
{
    int idx = split_light(slot);
    g_uniform.light_state |= 1 << idx;
}

/********************
 * sr_light_disable *
 ********************/

/* disables light at specified slot */
extern void 
sr_light_disable(enum sr_light slot)
{
    int idx = split_light(slot);
    g_uniform.light_state &= ~(1 << idx);
}

/***************
 * sr_material *
 ***************/

/* binds a material to pipeline */
extern void
sr_material(enum sr_light_attr attr, float* data)
{
    switch(attr) {
        case SR_AMBIENT:
            memcpy(g_material.ambient, data, 4 * sizeof(float));
            break;
        case SR_DIFFUSE:
            memcpy(g_material.diffuse, data, 4 * sizeof(float));
            break;
        case SR_SPECULAR:
            memcpy(g_material.specular, data, 4 * sizeof(float));
            break;
        case SR_BLEND:
            g_material.blend = *data;
            break;
        case SR_SHININESS:
            g_material.shininess = *data;
            break;
        default:
            return;
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
    vec3_sub(w, eye, look);
    normalize(w);

    /* side vector, u */
    float u[3];
    cross(u, up, w);   
    normalize(u);

    /* new up vector, v */
    float v[3];
    float up_proj_w[3]; /* up projected onto w */
    vec3_scale(up_proj_w, w, dot(up, w));
    vec3_sub(v, up, up_proj_w);
    normalize(v);
    
    struct mat4 m = {
        u[0], u[1], u[2], 0,
        v[0], v[1], v[2], 0,
        w[0], w[1], w[2], 0,
        0,    0,    0,    1
    };

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