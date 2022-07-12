#ifndef SR_CLIP_H
#define SR_CLIP_H

#include "sr_config.h"
#include "sr.h"


#if UNIT_TEST
#define u_static  
#else
#define u_static static
#endif



#if UNIT_TEST

void
lerp(float* from, float* to, float* dest, float a, size_t num_attr);

int
clip_routine(float* src, 
             float* dest, 
             size_t* num_pts, 
             size_t num_attr, 
             size_t axis, 
             int sign);
#endif

#endif /* SR_CLIP_H */