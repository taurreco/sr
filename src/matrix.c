
/**
* Copyright 2022 Tomas Dougan
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*    http://www.apache.org/licenses/LICENSE-2.0
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <stdlib.h>
#include "matrix.h"

/**
 * Maps a 2d representation of a matrix to 1d.
 * @param row row offset
 * @param col col offset
 * @param mat working matrix
 * @return index of 1d data field which corresponds to the
 * passed row and col.
 */
 static uint_t idx_map(uint_t row, uint_t col, const mat_t* mat) {
   return row * mat->num_cols + col;
 }

/**
 * Returns the number of elements in the matrix.
 * @param mat working matrix
 * @return number of elements in the matrix
 */

 static uint_t mat_size(const mat_t* mat) {
   return mat->num_rows * mat->num_cols;
 }

/*************
 * mat_alloc *
 *************/

mat_t* mat_alloc(uint_t num_rows, uint_t num_cols) {
  
  /* memory allocation */
  mat_t* mat = (mat_t*) malloc(sizeof(mat_t));
  float* data = (float*) malloc(sizeof(float) * num_rows * num_cols);
  
  /* field setting */
  mat->num_rows = num_rows;
  mat->num_cols = num_cols;
  mat->data = data;

  return mat;
}

/***********
 * mat_new *
 ***********/

mat_t* mat_new(uint_t num_rows, uint_t num_cols, const float* data) {
  mat_t* mat = mat_alloc(num_rows, num_cols);
  for (int i = 0; i < num_rows * num_cols; i++) {
    mat->data[i] = data[i];
  }
  return mat;
}

/************
 * mat_free *
 ************/

void mat_free(mat_t* mat) {
  free(mat->data);
  free(mat);
}

/**********
 * mat_at *
 **********/

 float mat_at(uint_t row, uint_t col, const mat_t* mat) {
   return mat->data[idx_map(row, col, mat)];
 }

/************
 * mat_at_r *
 ************/

 float* mat_at_r(uint_t row, uint_t col, mat_t* mat) {
   return mat->data + idx_map(row, col, mat);
 }

 /**************
  * mat_memset *
  **************/

 void mat_memset(mat_t* mat, const float val) {
   for (int i = 0; i < mat_size(mat); i++) {
     mat->data[i] = val;
   }
 }

/**************
 * mat_memcpy *
 **************/

mat_t* mat_memcpy(const mat_t* mat) {
  mat_t* cpy = mat_new(mat->num_rows, mat->num_cols, mat->data);
  return cpy;
}

/**********
 * mat_eq *
 **********/

int mat_eq(const mat_t* mat_l, const mat_t* mat_r) {
  if ((mat_l->num_rows != mat_r->num_rows) ||
      (mat_l->num_cols != mat_r->num_cols)) {
    return 0;
  }

  for (int i = 0; i < mat_l->num_rows * mat_l->num_cols; i++) {
    if (mat_l->data[i] != mat_r->data[i]) {
      return 0;
    }
  }
  return 1;
}

/***********
 * mat_mul *
 ***********/

mat_t* mat_mul(const mat_t* mat_l, const mat_t* mat_r) {
  
  if (mat_l->num_cols != mat_r->num_rows) { return NULL; }

  mat_t* mat_dest = mat_alloc(mat_l->num_rows, mat_r->num_cols);
  mat_memset(mat_dest, 0);

  for (int r = 0; r < mat_l->num_rows; r++) {
    for (int c = 0; c < mat_r->num_cols; c++) {
      for (int i = 0; i < mat_l->num_cols; i++) {
	    float val = mat_at(r, i, mat_l) * mat_at(i, c, mat_r);
        *mat_at_r(r, c, mat_dest) += val;
      }
    }
  }
  return mat_dest;
}
