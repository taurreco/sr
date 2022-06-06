
#include "pipeline.h"
#include "rasterize.h"

u_static void ss_map(sr_framebuffer_t* fb, float* x, float* y) {
  *x = *x + 1 * (fb->WIDTH / 2);
  *y = *y + 1 * (-fb->HEIGHT / 2);
}

