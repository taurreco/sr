#include <string.h>

#include "sr_config.h"
#include "sr.h"
#include "sr_clip.h"

int
clip_axis(float* src, 
          float* dest, 
          size_t* num_pts, 
          size_t num_attr, 
          size_t axis)
{
    float tmp[16 * 32];
    if (!clip_routine(src, tmp, num_pts, num_attr, axis, 1)) {
        return 0;
    }
    if (!clip_routine(tmp, dest, num_pts, num_attr, axis, -1)) {
        return 0;
    }
    return 1;
}

int
clip(float* src, float* dest, size_t* num_pts, size_t num_attr)
{
    /*
    int v0_inside = in_frustum(src + 0 * num_attr);
    int v1_inside = in_frustum(src + 1 * num_attr);
    int v2_inside = in_frustum(src + 2 * num_attr);
    

    if (v0_inside && v1_inside && v2_inside) {
        return 1;
    }

    if (!v0_inside && !v1_inside && !v2_inside) {
        return 0;
    }
    */
    float tmp[16 * 32];
    return clip_axis(src, dest, num_pts, num_attr, 0) &&
           clip_axis(dest, tmp, num_pts, num_attr, 1) &&
           clip_axis(tmp, dest, num_pts, num_attr, 2);
}


u_static int
clip_routine(float* src, 
             float* dest, 
             size_t* num_pts, 
             size_t num_attr, 
             size_t axis, 
             int sign)
{
    size_t k = 0;
    int poly_inside = 0;
    float* prev = src + (*num_pts - 1) * num_attr;  /* last pt */
    int prev_inside = prev[axis] * sign <= prev[3];
    for (size_t i = 0; i < *num_pts; i++) {
        float* curr = src + i * num_attr;
        int curr_inside = curr[axis] * sign <= curr[3];

        if (curr_inside ^ prev_inside) {
            float a = (prev[3] - prev[axis] * sign) / 
                      ((prev[3] - prev[axis] * sign) - 
                      (curr[3] - curr[axis] * sign));
            lerp(prev, curr, dest + k * num_attr, a, num_attr);
            k++;
        }

        if (curr_inside) {
            memcpy(dest + k * num_attr, curr, num_attr * sizeof(float));
            k++;
            poly_inside = 1;
        }
        prev = curr;
        prev_inside = curr_inside;
    }
    *num_pts = k;
    return poly_inside;
}

u_static void
lerp(float* from, float* to, float* dest, float a, size_t num_attr)
{
    for (size_t i = 0; i < num_attr; i++) {
        dest[i] = from[i] + (to[i] - from[i]) * a; // ?
    }
}