
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

/********
 * argb *
 ********/

/* arbg color representation */
uint32_t argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b) { 
    return (a<<24) | (r << 16) | (g << 8) | (b << 0); 
}

/*********************************************************************
 *                                                                   *
 *                               color                               *
 *                                                                   *
 *********************************************************************/

/**
 * in (7):
 *    float x, y, z;
 *    int a, r, g, b
 * 
 * out (8):
 *    float x, y, z, w;
 *    int a, r, g, b
 */

/************
 * color_vs *
 ************/

/* copies argb coords over from 'in' to 'out' */
static void 
color_vs(float* out, float* in, void* uniform)
{
    clip_space(out, in, uniform);
    memcpy(out + 4, in + 3, 4 * sizeof(float));
}

/************
 * color_fs *
 ************/

/* uses argb coords to fit color representation */
static void
color_fs(uint32_t* out, float* in, void* uniform)
{   
    int a = (int)in[3];
    int r = (int)in[4];
    int g = (int)in[5];
    int b = (int)in[6];

    *out = (a << 24) | (r << 16) | (g << 8) | (b << 0); 
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
    sr_bind_vs(color_vs, 9);
}

extern void
sr_bind_color_fs()
{
    sr_bind_fs(color_fs);
}

