
#include "sr_pipeline.h"
#include "config.h"

#ifndef PIPELINE_H
#define PIPELINE_H

#if UNIT_TEST
#define u_static  
#else
#define u_static static
#endif

#if UNIT_TEST
void ss_map(sr_framebuffer_t* fb, float* x, float* y);
#endif

#endif