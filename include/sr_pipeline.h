
#include <stdint.h>
#include <stddef.h>

#ifndef SR_PIPELINE_H
#define SR_PIPELINE_H

#define POINT 0b01
#define LINE 0b10
#define TRIANGLE 0b11

typedef float* point_t;
typedef float* line_t;
typedef float* triangle_t;

typedef struct sr_framebuffer {
  uint32_t HEIGHT;
  uint32_t WIDTH;
  uint32_t* color;
  float* depth;
} sr_framebuffer_t;

typedef struct sr_primitive {
  const uint8_t TYPE;
  point_t vertices;
} sr_primitive_t;

typedef struct sr_idxtrlst {
  point_t vertices;
  uint32_t* indices;
  uint32_t num_attr;
  uint32_t num_vertices;
  uint32_t num_indices;
} sr_idxtrlst_t;

typedef void (*sr_vertex_shader)(void*, point_t*, int*);
typedef void (*sr_geometry_shader)(void*, sr_primitive_t**, int*);
typedef void (*sr_fragment_shader)(void*, point_t*);

typedef struct sr_pipeline { /* sr_renderer */
  size_t num_attr;
  void* uniform_data;
  sr_vertex_shader vs;
  sr_geometry_shader gs;
  sr_fragment_shader fs;
  sr_framebuffer_t fb;
} sr_pipeline_t;

void sr_render(sr_pipeline_t* pipeline, sr_idxtrlst_t* obj);

#endif