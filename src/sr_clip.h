#ifndef SR_CLIP_H
#define SR_CLIP_H

#include "sr.h"

void
clip_poly(float* src, int* num_pts, 
          int num_attr, uint8_t clip_flags);

void
clip_test(float* pt, uint8_t* flags);

#endif /* SR_CLIP_H */