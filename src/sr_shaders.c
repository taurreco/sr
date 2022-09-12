
#include <stdlib.h>

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

/***********
 * rgb_int *
 ***********/

/* converts a float representation of a color into an int */
static uint32_t
rgb_int(float* color)
{
    uint8_t a = 255;
    uint8_t r = floorf(color[0] * 255);
    uint8_t g = floorf(color[1] * 255);
    uint8_t b = floorf(color[2] * 255);

    return a << 24 | r << 16 | g << 8 | b << 0; 
}

/*************
 * rgb_float *
 *************/

/* converts one int into a float representation of a color */
static void
rgb_float(float* a, uint32_t b)
{
    a[0] = ((b & 0x00FF0000) >> 16) / (float)255;
    a[1] = ((b & 0x0000FF00) >> 8) / (float)255;
    a[2] = (b & 0x000000FF) / (float)255;
}

/******************
 * blend_multiply *
 ******************/

/** 
 * multiplies a bottom layer, 'a', and top layer, 'b', 
 * and stores result in 'a' 
 */
static void 
blend_multiply(float* a, float* b)
{
    a[0] = fmin(a[0] * b[0], 1);
    a[1] = fmin(a[1] * b[1], 1);
    a[2] = fmin(a[2] * b[2], 1);
}

/***************
 * blend_scale *
 ***************/

/* scales the color 'a' by some intensity 'b' */
static void
blend_scale(float* a, float b)
{
    a[0] = fmin(a[0] * b, 1);
    a[1] = fmin(a[1] * b, 1);
    a[2] = fmin(a[2] * b, 1);
}

/*************
 * blend_add *
 *************/

/* combines two colors, stores the sum in 'a' */
static void
blend_add(float* a, float* b)
{
    a[0] = fmin(a[0] + b[0], 1);
    a[1] = fmin(a[1] + b[1], 1);
    a[2] = fmin(a[2] + b[2], 1);
}

/*********************************************************************
 *                                                                   *
 *                          shader helpers                           *
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
clip_space(float* out, float* in, struct sr_uniform* sr_uniform)
{
    float tmp[4] = {    /* homogenize in vector */
        in[0], in[1], in[2], 1
    };
    matmul_v(out, sr_uniform->mvp, tmp);
}

/******************
 * sample_texture *
 ******************/

/* returns the color of the texture at coordinates 'u' and 'v' */
static uint32_t
sample_texture(struct sr_texture* texture, float u, float v)
{
    int x = floorf(u * texture->width);
    int y = texture->height - floorf(v * texture->height);

    return texture->colors[y * texture->width + x];
}

/*********
 * phong *
 *********/

/**
 * given a world position, normal, and light, calculates 
 * an ambient, diffuse, and specular intensites to blend 
 * with the base color 
 */
static void
phong(float* final_color, struct sr_uniform* sr_uniform, 
      struct light* light, float* 
      pos, float* norm, float* base_color)
{

    /* colors */
    float ambient[3];
    float diffuse[3];
    float specular[3];

    /* vectors */
    float light_dir[3];
    float reflection_dir[3];
    float view_dir[3];

    normalize(sr_uniform->cam_pos);

    /* initialize color values */
    memcpy(ambient, light->color, 3 * sizeof(float));
    memcpy(diffuse, light->color, 3 * sizeof(float));
    memcpy(specular, light->color, 3 * sizeof(float));
    memset(final_color, 0, 3 * sizeof(float));

    /* initialize vectors */
    memcpy(view_dir, sr_uniform->cam_pos, 3 * sizeof(float));

    /* ambient */
    blend_scale(ambient, light->ambient);

    /* diffuse */
    sub_v(light_dir, light->pos, pos);   /* calculate light_dir */
    float dist = magnitude(light_dir);   /* only used for attenuation */

    normalize(light_dir);
    normalize(norm);
    
    blend_scale(diffuse, fmax(dot(norm, light_dir), 0));
    blend_scale(diffuse, light->diffuse);

    /* specular */
    float damp = 10;
    reflect_v(reflection_dir, norm, light_dir);   /* calculate reflection_dir */
    normalize(reflection_dir);

    float shine = fmax(dot(reflection_dir, view_dir), 0);
    shine = powf(shine, damp);

    blend_scale(specular, shine);

    /* attenuation */
    float attenuation = 1 / (light->attn_const +
                             light->attn_lin * dist + 
                             light->attn_quad * dist * dist);

    blend_scale(ambient, attenuation * 5);
    blend_scale(diffuse, attenuation * 5);

    /* combine all colors, then blend */
    blend_add(final_color, ambient);
    blend_add(final_color, diffuse);
    blend_add(final_color, specular);
    blend_multiply(final_color, base_color);
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
 * out (7):
 *    float x, y, z, w;
 *    float r, g, b;
 */

/************
 * color_vs *
 ************/

/* copies argb coords over from 'in' to 'out' */
static void 
color_vs(float* out, float* in, void* uniform)
{
    clip_space(out, in, uniform);
    memcpy(out + 4, in + 3, 3 * sizeof(float));
}

/************
 * color_fs *
 ************/

/* uses argb coords to fit color representation */
static void
color_fs(uint32_t* out, float* in, void* uniform)
{   
    *out = rgb_int(in + 4);
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

/********************
 * phong_texture_fs *
 ********************/

/* blends a phong sample with texture base */
static void
phong_texture_fs(uint32_t* out, float* in, void* uniform)
{   
    struct sr_uniform* sr_uniform = (struct sr_uniform*)uniform;

    float* pos = in + 4;
    float* norm = in + 9;
    float base_color[3];
    float final_color[3];
    
    rgb_float(base_color, sample_texture(sr_uniform->texture, in[7], in[8]));
    memset(final_color, 0, 3 * sizeof(float));

    for (int i = 0; i < SR_MAX_LIGHT_COUNT; i++) {
        if (sr_uniform->light_state & (1 << i)) {
            struct light light = sr_uniform->lights[i];
            float phong_color[3];
            phong(phong_color, sr_uniform, &light, 
                  pos, norm, base_color);
            blend_add(final_color, phong_color);
        }
    }
    *out = rgb_int(final_color);    
}

/******************
 * phong_color_fs *
 ******************/

/* blends a phong sample with color base */
static void
phong_color_fs(uint32_t* out, float* in, void* uniform)
{   
    struct sr_uniform* sr_uniform = (struct sr_uniform*)uniform;

    float* pos = in + 4;
    float* norm = in + 9;
    float base_color[3];
    float final_color[3];

    memcpy(base_color, sr_uniform->base_color, 3 * sizeof(float));
    memset(final_color, 0, 3 * sizeof(float));

    for (int i = 0; i < SR_MAX_LIGHT_COUNT; i++) {
        if (sr_uniform->light_state & (1 << i)) {
            struct light light = sr_uniform->lights[i];
            float phong_color[3];
            phong(phong_color, sr_uniform, &light, 
                  pos, norm, base_color);
            blend_add(final_color, phong_color);
        }
    }
    *out = rgb_int(final_color);
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
    sr_bind_vs(color_vs, 7);
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
sr_bind_phong_texture_fs()
{
    sr_bind_fs(phong_texture_fs);
}

extern void
sr_bind_phong_color_fs()
{
    sr_bind_fs(phong_color_fs);
}
