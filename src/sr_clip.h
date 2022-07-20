#ifndef SR_CLIP_H
#define SR_CLIP_H

#include "sr_config.h"
#include "sr.h"

#if UNIT_TEST
#define u_static  
#else
#define u_static static
#endif

/*********************************************************************
 *                                                                   *
 *                      public declarations                          *
 *                                                                   *
 *********************************************************************/

void
clip_poly(float* src, 
          size_t* num_pts, 
          size_t num_attr, 
          uint8_t clip_flags);

void
clip_test(float* pt, uint8_t* flags);

/*********************************************************************
 *                                                                   *
 *                      private declarations                         *
 *                                                                   *
 *********************************************************************/

#if UNIT_TEST

void
clip_routine(float* src, 
             float* dest, 
             size_t* num_pts, 
             size_t num_attr, 
             size_t axis, 
             int sign);

void
lerp(float* from, float* to, float* dest, float a, size_t num_attr);

void
swap(float** a, float** b);

#endif


#endif /* SR_CLIP_H */