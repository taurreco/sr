
#include "sr.h"
#include "sr_lib.h"
#include "sr_math.h"

/**
 * sr_shaders.c
 * --------
 * vertex and fragment shaders
 * 
 */

/*********************************************************************
 *                                                                   *
 *                          color & blending                         *
 *                                                                   *
 *********************************************************************/

/********
 * argb *
 ********/

static uint32_t
rgb_int(float* color)
{
    uint8_t a = 255;
    uint8_t r = floorf(color[0] * 255);
    uint8_t g = floorf(color[1] * 255);
    uint8_t b = floorf(color[2] * 255);

    return a << 24 | r << 16 | g << 8 | b << 0; 
}

static void
rgb_float(float* a, uint32_t b)
{
    a[0] = ((b & 0x00FF0000) >> 16) / (float)255;
    a[1] = ((b & 0x0000FF00) >> 8) / (float)255;
    a[2] = (b & 0x000000FF) / (float)255;
}

static void 
blend_multiply(float* a, float* b)
{
    a[0] = fmin(a[0] * b[0], 1);
    a[1] = fmin(a[1] * b[1], 1);
    a[2] = fmin(a[2] * b[2], 1);
}

static void
blend_scale(float* a, float b)
{
    a[0] = fmin(a[0] * b, 1);
    a[1] = fmin(a[1] * b, 1);
    a[2] = fmin(a[2] * b, 1);
}


static void
blend_add(float* a, float* b)
{
    a[0] = fmin(a[0] + b[0], 1);
    a[1] = fmin(a[1] + b[1], 1);
    a[2] = fmin(a[2] + b[2], 1);
}

static void
reflect(float* out, float* norm, float* incident)
{
    scale_v(out, norm, 2 * dot(incident, norm));
    sub_v(out, out, incident);
}


/*********************************************************************
 *                                                                   *
 *                              helpers                              *
 *                                                                   *
 *********************************************************************/

/**************
 * clip_space *
 **************/

/**
 * sends the vector 'in' to clip space by applying an mvp 
 * sets the first four indices of 'out' to these coordinates
 */
static void
clip_space(float* out, float* in, struct sr_uniform* uniform)
{
    float in_h[4] = {
        in[0], in[1], in[2], 1
    };
    matmul_v(out, uniform->mvp, in_h);
}

/******************
 * sample_texture *
 ******************/

static uint32_t
sample_texture(struct texture* texture, float u, float v)
{
    int x = floorf(u * texture->width);
    int y = texture->height - floorf(v * texture->height);

    return texture->colors[y * texture->width + x];
}

/*********
 * phong *
 *********/

static uint32_t 
phong(struct sr_uniform* uniform, 
      struct sr_point_light* light, 
      float* pos, float* norm, float* base_color)
{

    float ambient[3];
    float diffuse[3];
    float specular[3];
    float final_color[3];
    float light_dir[3];
    float incident_dir[3];
    float reflection_dir[3];

    normalize(uniform->cam_pos);

    memcpy(ambient, light->color, 3 * sizeof(float));
    memcpy(diffuse, light->color, 3 * sizeof(float));
    memcpy(specular, light->color, 3 * sizeof(float));
    memset(final_color, 0, 3 * sizeof(float));

    /* ambient */
    blend_scale(ambient, 0.3);

    /* diffuse */
    sub_v(light_dir, light->pos, pos);

    float dist = magnitude(light_dir);   /* only used for attenuation */

    normalize(light_dir);
    normalize(norm);

    float diff = fmax(dot(norm, light_dir), 0);
    
    blend_scale(diffuse, diff);

    /* specular */
    float damp = 10;
    reflect(reflection_dir, norm, light_dir);
    normalize(reflection_dir);

    float shine = fmax(dot(reflection_dir, uniform->cam_pos), 0);
    shine = powf(shine, damp);

    blend_scale(specular, shine * 1);

    /* attenuation */

    float attenuation = 1 / (light->const_attn +
                             light->lin_attn * dist + 
                             light->quad_attn * dist * dist);

    blend_scale(ambient, attenuation * 30);
    blend_scale(diffuse, attenuation * 30);

    blend_add(final_color, ambient);
    blend_add(final_color, diffuse);
    blend_add(final_color, specular);
    blend_multiply(final_color, base_color);
    
    return rgb_int(final_color);
}

/*********************************************************************
 *                                                                   *
 *                               color                               *
 *                                                                   *
 *********************************************************************/

/**
 * in (3):
 *    float x, y, z;
 * 
 * out (10):
 *    float x, y, z, w;
 *    float wx, wy, wz;
 *    float nx, ny, nz;
 */

/************
 * color_vs *
 ************/

/* copies argb coords over from 'in' to 'out' */
static void 
color_vs(float* out, float* in, void* uniform)
{
    struct sr_uniform* sr_uniform = (struct sr_uniform*)uniform;
    clip_space(out, in, uniform);
    matmul_v(out + 4, sr_uniform->model, in);
    matmul_v(out + 7, sr_uniform->model, in);
    normalize(out + 7);
}

/************
 * color_fs *
 ************/

/* uses argb coords to fit color representation */
static void
color_fs(uint32_t* out, float* in, void* uniform)
{   
    struct sr_uniform* sr_uniform = (struct sr_uniform*)uniform;
    struct sr_point_light* light = sr_uniform->light;

    float* pos = in + 4;
    float* norm = in + 7;
    *out = phong(sr_uniform, light, pos, norm, sr_uniform->base_color);
}

/*********************************************************************
 *                                                                   *
 *                              texture                              *
 *                                                                   *
 *********************************************************************/

/**
 * in (5):
 *    float x, y, z;
 *    float u, v;
 * 
 * out (6):
 *    float x, y, z, w;
 *    float u, v;
 */

/**************
 * texture_vs *
 **************/

/* copies argb coords over from 'in' to 'out' */
static void 
texture_vs(float* out, float* in, void* uniform)
{
    clip_space(out, in, uniform);
    memcpy(out + 4, in + 3, 2 * sizeof(float));
}

/**************
 * texture_fs *
 **************/

/* uses argb coords to fit color representation */
static void
texture_fs(uint32_t* out, float* in, void* uniform)
{   
    struct sr_uniform* sr_uniform = (struct sr_uniform*)uniform;

    *out = sample_texture(sr_uniform->texture, in[4], in[5]);
}


/*********************************************************************
 *                                                                   *
 *                               phong                               *
 *                                                                   *
 *********************************************************************/

/**
 * in (8):
 *    float x, y, z;
 *    float u, v;
 *    float nx, ny, nz;
 * 
 * out (12):
 *    float x, y, z, w;
 *    float wx, wy, wz;
 *    float u, v;
 *    float nx, ny, nz;
 */

/************
 * phong_vs *
 ************/

/* copies argb coords over from 'in' to 'out' */
static void 
phong_vs(float* out, float* in, void* uniform)
{
    struct sr_uniform* sr_uniform = (struct sr_uniform*)uniform;

    /* calculate x, y, z, w */
    clip_space(out, in, sr_uniform);

    /* calculate wx, wy, wz */
    matmul_v(out + 4, sr_uniform->model, in);

    /* copy over texture cordinates */
    memcpy(out + 7, in + 3, 2 * sizeof(float));

    /* send normals to world space */    
    matmul_v(out + 9, sr_uniform->model, in + 5);

    /* normalize them */
    normalize(out + 9);
}


/************
 * phong_fs *
 ************/

/* uses argb coords to fit color representation */
static void
phong_fs(uint32_t* out, float* in, void* uniform)
{   
    struct sr_uniform* sr_uniform = (struct sr_uniform*)uniform;

    float* pos = in + 4;
    float* norm = in + 9;
    float base_color[3];
    
    rgb_float(base_color, sample_texture(sr_uniform->texture, in[7], in[8]));
    *out = phong(sr_uniform, sr_uniform->light, pos, norm, base_color);
}


/* uses argb coords to fit color representation */
static void
phong_color_fs(uint32_t* out, float* in, void* uniform)
{   
    struct sr_uniform* sr_uniform = (struct sr_uniform*)uniform;

    float* pos = in + 4;
    float* norm = in + 9;
    float base_color[3];

 //   printf("%f %f %f\n", sr_uniform->cam_pos[0], sr_uniform->cam_pos[1], sr_uniform->cam_pos[2]);

    *out = phong(sr_uniform, sr_uniform->light, pos, norm, sr_uniform->base_color);
}


/*********************************************************************
 *                                                                   *
 *                             bindings                              *
 *                                                                   *
 *********************************************************************/

/*********
 * color *
 *********/

extern void
sr_bind_color_vs()
{
    sr_bind_vs(color_vs, 10);
}

extern void
sr_bind_color_fs()
{
    sr_bind_fs(color_fs);
}

/***********
 * texture *
 ***********/

extern void
sr_bind_texture_vs()
{
    sr_bind_vs(texture_vs, 6);
}

extern void
sr_bind_texture_fs()
{
    sr_bind_fs(texture_fs);
}

/*********
 * phong *
 *********/

extern void
sr_bind_phong_vs()
{
    sr_bind_vs(phong_vs, 12);
}

extern void
sr_bind_phong_fs()
{
    sr_bind_fs(phong_fs);
}

extern void
sr_bind_phong_color_fs()
{
    sr_bind_fs(phong_color_fs);
}




