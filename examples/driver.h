#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <stdint.h>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern uint32_t* colors;
extern float* depths;

extern void
start();

extern void
update(float dt);

extern void
end();

#endif /* EXAMPLE_H */