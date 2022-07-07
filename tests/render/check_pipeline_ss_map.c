


#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr.h"


void setUp() {}

void tearDown() {}

/**************
 * check_ss *
 **************/

void check_ss() {

    sr_framebuffer_t fb;
    fb.WIDTH = 100.0;
    fb.HEIGHT = 100.0;
    fb.color = NULL;
    fb.depth = NULL;

    float x, y;
    x = 1.0;
    y = 2.0;

    ss_map(&fb, &x, &y);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(check_ss);
    return UNITY_END();
}
