#include <string.h>

#include "sr_config.h"
#include "sr.h"
#include "sr_clip.h"

/*********************************************************************
 *                                                                   *
 *                       private declarations                        *
 *                                                                   *
 *********************************************************************/
#if !(UNIT_TEST)

u_static void
lerp(float* from, float* to, float* dest, float a, size_t num_attr);

#endif

/*********************************************************************
 *                                                                   *
 *                         public definition                         *
 *                                                                   *
 *********************************************************************/

/*************
 * clip_poly *
 *************/

/**
 * matches a one-hot clip flag to its clip routine
 * updates source points to the clipped ones in place
 */
void
clip_poly(float* src, 
          size_t* num_pts, 
          size_t num_attr, 
          uint8_t clip_flags)
{
    float dest[16 * SR_MAX_ATTRIBUTE_COUNT];
    float tmp[16 * SR_MAX_ATTRIBUTE_COUNT];
    memcpy(tmp, src, *num_pts * num_attr);

    if (clip_flags & SR_CLIP_LEFT_PLANE) {    /* left */
        clip_routine(tmp, dest, num_pts, num_attr, 0, -1);
        memcpy(tmp, dest, *num_pts * num_attr);
    }

    if (clip_flags & (SR_CLIP_BOTTOM_PLANE)) {    /* bottom */
        clip_routine(tmp, dest, num_pts, num_attr, 1, -1);
        memcpy(tmp, dest, *num_pts * num_attr);
    }

    if (clip_flags & SR_CLIP_NEAR_PLANE) {    /* near */
        clip_routine(tmp, dest, num_pts, num_attr, 2, -1);
        memcpy(tmp, dest, *num_pts * num_attr);
    }

    if (clip_flags & SR_CLIP_RIGHT_PLANE) {    /* right */
        clip_routine(tmp, dest, num_pts, num_attr, 0, 1);
        memcpy(tmp, dest, *num_pts * num_attr);
    }

    if (clip_flags & SR_CLIP_TOP_PLANE) {    /* top */
        clip_routine(tmp, dest, num_pts, num_attr, 1, 1);
        memcpy(tmp, dest, *num_pts * num_attr);
    }

    memcpy(src, dest, *num_pts * num_attr);
}

/****************
 * clip_routine *
 ****************/

/**
 * a variant of cohen-sutherland for one plane
 * as determined by an axis and a sign to indicate direction
 */
static void
clip_routine(float* src, 
             float* dest, 
             size_t* num_pts, 
             size_t num_attr, 
             size_t axis, 
             int sign)
{
    size_t k = 0;

    float* prev = src + (*num_pts - 1) * num_attr;  /* last pt */
    int prev_inside = prev[axis] * sign <= prev[3];

    for (size_t i = 0; i < *num_pts; i++) {

        float* curr = src + i * num_attr;
        int curr_inside = curr[axis] * sign <= curr[3];

        /* intersection */

        if (curr_inside ^ prev_inside) {
            float a = (prev[3] - prev[axis] * sign) / 
                      ((prev[3] - prev[axis] * sign) - 
                      (curr[3] - curr[axis] * sign));
            lerp(prev, curr, dest + k * num_attr, a, num_attr);
            k++;
        }

        /* point inside */

        if (curr_inside) {
            memcpy(dest + k * num_attr, curr, num_attr * sizeof(float));
            k++;
        }
        
        prev = curr;
        prev_inside = curr_inside;
    }
    *num_pts = k;    /* new number of points after clip */
}

/*************
 * clip_test *
 *************/

/* assigns proper clip flags to a point */
void
clip_test(float* pt, uint8_t* flags)
{
    uint8_t left = (pt[3] + pt[0] < 0) << 0;
    uint8_t bottom = (pt[3] + pt[1] < 0) << 1;
    uint8_t near = (pt[3] + pt[2] < 0) << 2;
    uint8_t right = (pt[3] - pt[0] < 0) << 3;
    uint8_t top = (pt[3] - pt[1] < 0) << 4;

    *flags = left | bottom | near | right | top;
}

/*********************************************************************
 *                                                                   *
 *                       private definitions                         *
 *                                                                   *
 *********************************************************************/

/********
 * lerp *
 ********/

/* interpolates a point some length up an edge */
u_static void
lerp(float* from, float* to, float* dest, float a, size_t num_attr)
{
    for (size_t i = 0; i < num_attr; i++) {
        dest[i] = from[i] + (to[i] - from[i]) * a;
    }
}