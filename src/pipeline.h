#include "common.h"

#ifndef SR_PIPELINE_H
#define SR_PIPELINE_H

#if UNIT_TEST
#define u_static  
#else
#define u_static static
#endif

#define POINT 0b01
#define LINE 0b10
#define TRIANGLE 0b11

typedef struct sr_framebuffer {
  const uint32_t HEIGHT;
  const uint32_t WIDTH;
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

#if UNIT_TEST
struct edge;
int is_top_edge(point_t tail, point_t tip);
float init_edge(struct edge* edge, point_t tail, point_t tip, point_t origin);
void swap_points(point_t* a, point_t* b);
void orient_ccw(point_t* v0, point_t* v1, point_t* v2);
void draw_point(sr_pipeline_t* pipeline, point_t point);
void draw_triangle(sr_pipeline_t* pipeline, triangle_t triangle);
#endif

#endif
