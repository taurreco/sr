#include <string.h>

#include "sr_config.h"
#include "sr.h"
#include "sr_clip.h"

int
clip_triangle(float* src, float* dest, size_t* num_pts, size_t num_attr)
{
    float tmp[16 * SR_MAX_ATTRIBUTE_COUNT];
    return clip_routine(src, tmp, num_pts, num_attr, 0, 1) &&
           clip_routine(tmp, dest, num_pts, num_attr, 0, -1) &&
           clip_routine(dest, tmp, num_pts, num_attr, 1, 1) &&
           clip_routine(tmp, dest, num_pts, num_attr, 1, -1) &&
           clip_routine(dest, tmp, num_pts, num_attr, 2, 1) &&
           clip_routine(tmp, dest, num_pts, num_attr, 2, -1);
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