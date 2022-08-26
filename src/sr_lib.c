#include "sr.h"
#include "sr_math.h"

struct sr_uniform {
    struct mat4* mvp;
};

const struct mat4 identity = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

struct mat4 model = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

struct mat4 view = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

struct mat4 proj = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

struct mat4 mvp = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

struct mat4* cur_mat;  /* points to whichever matrix stack is being used */

struct sr_framebuffer fixed_frame = {
    .width = 0,
    .height = 0,
    .colors = 0,
    .depths = 0
};

struct sr_uniform fixed_uniform = {
    .mvp = &mvp
};

struct sr_pipeline fixed_pipeline = {
    .fbuf = &fixed_frame,
    .uniform = (void*)(&fixed_uniform),
    .vs = 0,
    .fs = 0,
    .pts_in = 0,
    .n_pts = 0,
    .n_attr_in = 0,
    .n_attr_out = 8,
    .winding = SR_WINDING_ORDER_CCW
};

static void 
test_vs(void* uniform, float* in, float* out)
{
    struct mat4* mvp = ((struct sr_uniform*)uniform)->mvp;
    float hin[4] = {
        in[0], in[1], in[2], 1
    };
    matmul_v(out, mvp, hin);
    memcpy(out + 4, in + 3, 5 * sizeof(float));
}
static void
test_fs(void* uniform, float* in, uint32_t* out)
{
    *out = (0 << 24) | (158 << 16) | (85 << 8) | (194 << 0); 
    
    if ((int)in[6] == 0 && (int)in[7] == 1 && (int)in[8] == 0)
        *out = (0 << 24) | (151 << 16) | (151 << 8) | (151 << 0); /* grey */
    if ((int)in[6] == 0 && (int)in[7] == 0 && (int)in[8] == 1)
        *out = (0 << 24) | (86 << 16) | (194 << 8) | (138 << 0); 
    if ((int)in[6] == -1 && (int)in[7] == 0 && (int)in[8] == 0)
        *out = (0 << 24) | (162 << 16) | (134 << 8) | (38 << 0); 
    if ((int)in[6] == 0 && (int)in[7] == -1 && (int)in[8] == 0)
        *out = (0 << 24) | (20 << 16) | (121 << 8) | (176 << 0); 
    if ((int)in[6] == 1 && (int)in[7] == 0 && (int)in[8] == 0)
        *out = (0 << 24) | (43 << 16) | (29 << 8) | (164 << 0); 
    if ((int)in[6] == 0 && (int)in[7] == 0 && (int)in[8] == -1)
        *out = (0 << 24) | (198 << 16) | (7 << 8) | (163 << 0); 

   
}



/*********
 * cross *
 *********/

extern void
sr_fixed_draw(int* indices, int n_indices, uint8_t prim_type)
{
    mvp = identity;
    matmul(&mvp, &proj);
    matmul(&mvp, &view);
    matmul(&mvp, &model);
    sr_draw_indexed(&fixed_pipeline, indices, n_indices, prim_type);
}

/***************
 * sr_bind_pts *
 ***************/

/* sets points */
extern void
sr_bind_pts(float* pts, int n_pts, int n_attr)
{
    fixed_pipeline.pts_in = pts;
    fixed_pipeline.n_pts = n_pts;
    fixed_pipeline.n_attr_in = n_attr;
}

/***********************
 * sr_bind_framebuffer *
 ***********************/

/* attaches color and depth buffers to global framebuffer */
extern void
sr_bind_framebuffer(int width, int height, uint32_t* colors, float* depths)
{
    fixed_frame.width = width;
    fixed_frame.height = height;
    fixed_frame.colors = colors;
    fixed_frame.depths = depths;
}

/***************
 * sr_viewport *
 ***************/

/* specifies ndc to screen space transformation */
extern void
sr_viewport(int width, int height)
{
    fixed_frame.width = width;
    fixed_frame.height = height;
}

/**************
 * sr_bind_vs *
 **************/

extern void
sr_bind_vs(vs_f vs, int n_attr_out)
{
    fixed_pipeline.vs = vs;
    fixed_pipeline.n_attr_out = n_attr_out;
}

/**************
 * sr_bind_fs *
 **************/

extern void
sr_bind_fs(fs_f fs)
{
    fixed_pipeline.fs = fs;
}

extern void
sr_bind_vs_test()
{
    sr_bind_vs(test_vs, 9);
}

extern void
sr_bind_fs_test()
{
    sr_bind_fs(test_fs);
}

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

/*****************
 * sr_get_floats *
 *****************/

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

/********************
 * sr_load_identity *
 ********************/

/* sets current matrix to the identity */
extern void
sr_load_identity()
{
   *cur_mat = identity;
}

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

/* pushes a matrix which rolls by r radians */
extern void
sr_rotate_x(float r)
{
    float c = cos(r);
    float s = sin(r);

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

/* pushes a matrix which rotates around pitch by p radians */
extern void
sr_rotate_y(float p)
{
    float c = cos(p);
    float s = sin(p);

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

/* pushes a matrix which rotats arownd yaw by y radians */
extern void
sr_rotate_z(float y)
{
    float c = cos(y);
    float s = sin(y);
    
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
           float cx, float cy, float cz, 
           float ux, float uy, float uz)
{
    float f[3] = {
        cx - ex, cy - ey, cz - ez  /* forward vector */
    };

    float UP[3] = {   /* up vector */
        ux, uy, uz
    };

    normalize(f);
    normalize(UP);

    float s[3], u[3];

    cross(s, f, UP);   /* orthonormal to f and up */
    normalize(s);
    cross(u, s, f);  /* new up close to UP but orthogonal to f */
    
    struct mat4 m = {
        s[0],  s[1],  s[2],  0,
        u[0],  u[1],  u[2],  0,
        -f[0], -f[1], -f[2], 0,
        0,     0,     0,     1
    };

    matmul(cur_mat, &m);
    sr_translate(-ex, -ey, -ez);
}

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
        0,   0,   1,    0
    };

    matmul(cur_mat, &p);
}