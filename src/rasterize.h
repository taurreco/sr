
#include "sr_pipeline.h"
#include "config.h"

#ifndef RASTERIZE_H
#define RASTERIZE_H

#if UNIT_TEST
#define u_static  
#else
#define u_static static
#endif

void draw_point(sr_pipeline_t* pipeline, point_t point);
void draw_line(sr_pipeline_t* pipeline, line_t line);
void draw_triangle(sr_pipeline_t* pipeline, triangle_t triangle);

#if UNIT_TEST
struct edge;
int is_top_edge(point_t tail, point_t tip);
float init_edge(struct edge* edge, point_t tail, point_t tip, point_t origin);
void swap_points(point_t* a, point_t* b);
void orient_ccw(point_t* v0, point_t* v1, point_t* v2);
#endif

#endif