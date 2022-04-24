
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

#ifndef MAT_H
#define MAT_H

typedef unsigned int uint_t;

/*****************************************************
 *                                                   *
 *                       Data                        *
 *                                                   *
 *****************************************************/

/** The matrix struct, used to organizes matrix data */

typedef struct mat {
  uint_t num_rows; /* number of rows */
  uint_t num_cols; /* number of columns */
  float* data; /* data stored in one contiguous block */
} mat_t;


/*****************************************************
 *                                                   *
 *         Constructors and Deconstructors           *
 *                                                   *
 *****************************************************/

/**
 * Creates a matrix with an uninitialized 
 * (but allocated) data block.
 * @param num_rows number of rows
 * @param num_cols number of columns
 * @return an empty matrix
 */

mat_t* mat_alloc (uint_t num_rows, uint_t num_cols);

/**
 * Creates a matrix structure given all relevant data
 * @param num_rows number of rows
 * @param num_cols number of columns
 * @param data the values of the matrix
 * @return a matrix whose data is a copy of the passed data
 */

mat_t* mat_new (uint_t num_rows, uint_t num_cols, const float* data);

/**
 * Frees the data pointer of a given matrix, and the
 * memory allocated to the matrix structure itself.
 * @param mat is the pointer to the matrix which is to be
 * de-allocated
 */

void mat_free(mat_t* mat);


/*****************************************************
 *                                                   *
 *             Matrix Utility Functions              *
 *                                                   *
 *****************************************************/

 /**
  * Reads the value of a matrix at a 
  * specified position.
  * @param row value's row
  * @param col value's column
  * @param mat, given matrix
  * @return the value of mat at [row, col]
  */

float mat_at(uint_t row, uint_t col, const mat_t* mat);

/**
 * Accesses the value of a matrix at a
 * specified position.
 * @param row value's row
 * @param col values's col
 * @param mat, given matrix
 * @return the address of the value
 */

float* mat_at_r(uint_t row, uint_t col, mat_t* mat);

/**
 * Sets all the data in mat to val.
 * @param mat matrix to manipulate
 * @param data value to set
 */

void mat_memset(mat_t* mat, const float val);

/**
 * Copies a matrix.
 * @param mat matrix to be copied
 * @return matrix whose data is the same as mat
 * but belongs to its own memory address
 */

mat_t* mat_memcpy(const mat_t* mat);


/*****************************************************
 *                                                   *
 *                 Matrix Operations                 *
 *                                                   *
 *****************************************************/

/**
 * Determines whether two matrices are equal.
 * @param mat_l left hand matrix
 * @param mat_r right hand matrix
 * @return 1 if both are equal, 0 if not
 */

int mat_eq(const mat_t* mat_l, const mat_t* mat_r);

 /*****************************************************
  *                 Multiplication                    *
  *****************************************************/
/**
 * Multiplies two matricies.
 * @param mat_l left hand matrix
 * @param mat_r right hand matrix
 * @return resulting product
 */

mat_t* mat_mul(const mat_t* mat_l, const mat_t* mat_r);


/**
 * Scales a matrix by a given value.
 * @param val scalar value
 * @param mat matrix
 * @return resulting matrix
 */

mat_t* mat_scale(float val, const mat_t* mat);

 /*****************************************************
  *              Adition and Subtraction              *
  *****************************************************/

/**
 * Adds two matricies together.
 * @param mat_l left hand matrix
 * @param mat_r right hand matrix
 * @param mat_dest the destination matrix
 * @return whether the addition was succesful
 */

int mat_add(mat_t* mat_dest, const mat_t* mat_l, const mat_t* mat_r);


/**
 * Adds two matricies together.
 * @param mat_l left hand matrix
 * @param mat_r right hand matrix
 * @return resulting matrix
 */

mat_t* mat_add_r(const mat_t* mat_l, const mat_t* mat_r);

/**
 * Subtracts two matricies together.
 * @param mat_l left hand matrix
 * @param mat_r right hand matrix
 * @param mat_dest the destination matrix
 * @return whether the subtraction was succesful
 */

int mat_sub(mat_t* mat_dest, const mat_t* mat_l, const mat_t* mat_r);


/**
 * Subtracts two matricies together.
 * @param mat_l left hand matrix
 * @param mat_r right hand matrix
 * @return resulting matrix
 */

mat_t* mat_sub_r(const mat_t* mat_l, const mat_t* mat_r);


/*****************************************************
 *                                                   *
 *         Constructors and Deconstructors           *
 *                                                   *
 *****************************************************/

mat_t* mat_det(const mat_t* mat);


#endif /* MAT_H */
