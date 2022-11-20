
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver.h"
#include "sr.h"

float pts[9] = {
    0.5, 0.5, 0,
    -0.5, 0.5, 0,
    0, -0.5, 0,
};

int indices[3] = {
    0, 1, 2
};

static void 
vs(float* out, float* in, void* uniform)
{
    memcpy(out, in, 3 * sizeof(float));
    out[3] = 1;    /* homogenize */
}

static void
fs(uint32_t* out, float* in, void* uniform) 
{
    *out = 0xAFF18FFF;
}

extern void
start()
{
    sr_bind_pts(pts, 3, 3);
    sr_bind_framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, colors, depths);
    sr_bind_vs(vs, 4);
    sr_bind_fs(fs);
}

extern void
update(float dt)
{
    sr_renderl(indices, 3, SR_TRIANGLE_LIST);
}

extern void
end() { /* not implemented */ }