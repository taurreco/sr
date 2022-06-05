

#include "common.h"
#include "pipeline.h"
#include <math.h>

typedef struct edge {
  point_t tail, tip;
  float step_x, step_y;
  int is_top_left;
} edge_t;

u_static void screen_space_map(sr_framebuffer_t* fb, float* x, float* y) {
  *x = *x + 1 * (fb->WIDTH / 2);
  *y = *y + 1 * (-fb->HEIGHT / 2);
}

u_static int is_top_left(point_t tail, point_t tip) {
  int is_top = (tail[1] == tip[1]) && (tail[0] > tip[0]); /* horizontal and directed left */ 
  int is_left = tail[1] < tip[1]; /* directed down */
  return is_top || is_left;
}

u_static float init_edge(edge_t* edge, point_t tail, point_t tip, point_t origin) {
  
  edge->tail = tail;
  edge->tip = tip;
  edge->is_top_left = is_top_left(tail, tip);

  float A = tail[1] - tip[1];
  float B = tip[0] - tail[0];
  float C = tail[0] * tip[1] - tip[0] * tail[1]; 

  /* step sizes */
  edge->step_x = A;
  edge->step_y = B;

  /* edge function output at origin */
  return A * origin[0] + B * origin[1] + C;
}


u_static void swap_points(point_t* a, point_t* b) {
  point_t tmp = *a;
  *a = *b;
  *b = tmp;
}

u_static void orient_ccw(point_t* v0, point_t* v1, point_t* v2) {
  if (*v1[1] > *v0[1]) {
    swap_points(v0, v1);
  }
  if (*v2[1] > *v0[1]) {
    swap_points(v0, v2);
  }
  if (*v1[0] < *v2[0]) {
    swap_points(v1, v2);
  }
  if (*v1[0] == *v2[0] && *v1[1] < *v2[1]) {
    swap_points(v1, v2);
  }
}

u_static void draw_point(sr_pipeline_t* pipeline, point_t point) {
  pipeline->fs(pipeline->uniform_data, &point);
  size_t fb_idx = floorf(point[0]) * pipeline->fb.WIDTH + floorf(point[1]);
  if (point[2] > pipeline->fb.depth[fb_idx]) {   /* depth buffer */
    pipeline->fb.color[fb_idx] = point[3];
    pipeline->fb.depth[fb_idx] = point[2];
  }
}

u_static void draw_triangle(sr_pipeline_t* pipeline, triangle_t triangle) {

  /* orient triangle ccw */
  
  point_t v0 = (point_t)malloc(sizeof(float) * pipeline->num_attr);
  point_t v1 = (point_t)malloc(sizeof(float) * pipeline->num_attr);
  point_t v2 = (point_t)malloc(sizeof(float) * pipeline->num_attr);

  memcpy(&v0, triangle, pipeline->num_attr);
  memcpy(&v1, triangle + pipeline->num_attr, pipeline->num_attr);
  memcpy(&v2, triangle + (2 * pipeline->num_attr), pipeline->num_attr);

  orient_ccw(&v0, &v1, &v2);

  /* find bounding box */

  float min_x = fmin(v0[0], fmin(v1[0], v2[0]));
  float min_y = fmin(v0[1], fmin(v1[1], v2[1]));
  float max_x = fmax(v0[0], fmin(v1[0], v2[0]));
  float max_y = fmax(v0[1], fmin(v1[1], v2[1]));

  /* clip bounding box */

  min_x = fmax(min_x, 0);
  min_y = fmax(min_y, 0);
  max_x = fmin(max_x, pipeline->fb.WIDTH - 1);
  max_y = fmin(max_y, pipeline->fb.HEIGHT - 1);

  /* align to pixel centers */

  min_x = floorf(min_x) + 0.5;
  min_y = floorf(min_y) + 0.5;
  max_x = floorf(max_x) + 0.5;
  max_y = floorf(max_y) + 0.5;

  /* store current point */

  point_t p = (point_t)calloc(pipeline->num_attr, sizeof(float));
  p[0] = min_x;
  p[1] = min_y;

  /* grab edge information */

  edge_t e01, e12, e20;
  float w0 = init_edge(&e12, v0, v1, p);
  float w1 = init_edge(&e20, v0, v1, p);
  float w2 = init_edge(&e01, v0, v1, p);

  /* rasterize */

  for (p[1] = min_y; p[1] < max_y; p[1]++) {
    for (p[0] = min_x; p[0] < max_x; p[0]++) {

      float bias0 = (w0 == 0) && !e12.is_top_left ? -1 : 1;
      float bias1 = (w1 == 0) && !e20.is_top_left ? -1 : 1;
      float bias2 = (w2 == 0) && !e01.is_top_left ? -1 : 1;

      if (w0 * bias0 < 0 || w1 * bias1 < 0 || w2 * bias2 < 0) {
        /* interpolate attributes */
        for (int i = 2; i < pipeline->num_attr; i++) {
          p[i] = w0 * v0[i] + w1 * v1[i] + w2 * v2[i];
        }
        draw_point(pipeline, p);
      }

      w0 += e12.step_x;
      w1 += e20.step_x;
      w2 += e01.step_x;
    }
    w0 += e12.step_y;
    w1 += e20.step_y;
    w2 += e01.step_y;
  }

  free(v0);
  free(v1);
  free(v2);
  free(p);
}

