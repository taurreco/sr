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
clip_routine(float* src, float* dst, 
             size_t* n_pts, size_t n_attr, 
             size_t axis, int sign);

u_static void
lerp(float* from, float* to, float* dst, float a, size_t n_attr);

u_static void
swap(float** a, float** b);

#endif

/*********************************************************************
 *                                                                   *
 *                        public definitions                         *
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
clip_poly(float* src, size_t* n_pts, 
          size_t n_attr, uint8_t clip_flags)
{
    float dst[16 * SR_MAX_ATTRIBUTE_COUNT];
    float tmp[16 * SR_MAX_ATTRIBUTE_COUNT];
    memcpy(tmp, src, *n_pts * n_attr * sizeof(float));
    float* tmp_src = (float*)tmp;
    float* tmp_dst = (float*)dst;

    if (clip_flags & SR_CLIP_LEFT_PLANE) {    /* left */
        clip_routine(tmp_src, tmp_dst, n_pts, n_attr, 0, -1);
        swap(&tmp_src, &tmp_dst);
    }

    if (clip_flags & (SR_CLIP_BOTTOM_PLANE)) {    /* bottom */
         clip_routine(tmp_src, tmp_dst, n_pts, n_attr, 1, -1);
        swap(&tmp_src, &tmp_dst);
    }

    if (clip_flags & SR_CLIP_NEAR_PLANE) {    /* near */
        clip_routine(tmp_src, tmp_dst, n_pts, n_attr, 2, -1);
        swap(&tmp_src, &tmp_dst);
    }

    if (clip_flags & SR_CLIP_RIGHT_PLANE) {    /* right */
        clip_routine(tmp_src, tmp_dst, n_pts, n_attr, 0, 1);
        swap(&tmp_src, &tmp_dst);
    }

    if (clip_flags & SR_CLIP_TOP_PLANE) {    /* top */
        clip_routine(tmp_src, tmp_dst, n_pts, n_attr, 1, 1);
        swap(&tmp_src, &tmp_dst);
    }

    memcpy(src, tmp_src, *n_pts * n_attr * sizeof(float));
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

/****************
 * clip_routine *
 ****************/

/**
 * a variant of cohen-sutherland for one plane
 * as determined by an axis and a sign to indicate direction
 */
u_static void
clip_routine(float* src, float* dst, 
             size_t* n_pts, size_t n_attr, 
             size_t axis, int sign)
{
    size_t new_n_pts = 0;

    float* prev = src + (*n_pts - 1) * n_attr;  /* last pt */
    int prev_inside = prev[axis] * sign <= prev[3];

    for (size_t i = 0; i < *n_pts; i++) {

        float* curr = src + i * n_attr;
        int curr_inside = curr[axis] * sign <= curr[3];

        /* intersection */

        if (curr_inside ^ prev_inside) {
            float a = (prev[3] - prev[axis] * sign) / 
                      ((prev[3] - prev[axis] * sign) - 
                      (curr[3] - curr[axis] * sign));
            lerp(prev, curr, dst + new_n_pts * n_attr, 
                 a, n_attr);
            new_n_pts++;
        }

        /* point inside */

        if (curr_inside) {
            memcpy(dst + new_n_pts * n_attr, 
                   curr, n_attr * sizeof(float));
            new_n_pts++;
        }
        
        prev = curr;
        prev_inside = curr_inside;
    }
    *n_pts = new_n_pts;    /* new number of points after clip */
}

/********
 * lerp *
 ********/

/* interpolates a point some length up an edge */
u_static void
lerp(float* from, float* to, float* dst, float a, size_t n_attr)
{
    for (size_t i = 0; i < n_attr; i++) {
        dst[i] = from[i] + (to[i] - from[i]) * a;
    }
}

/********
 * swap *
 ********/

/* swaps what the pointers point to */
u_static void
swap(float** a, float** b)
{
    void* tmp = *a;
    *a = *b;
    *b = tmp;
}