#include <stdlib.h>
#include "unity.h"
#include "matrix.h"

void setUp(void) {}

void tearDown(void) {}

/**************************************************
 *                                                *
 *              check fml_mat_alloc               *
 *                                                *
 **************************************************/

 /* 1 by 1 */

void check_mat_alloc_1_by_1(void) {

  /* declaration */

  float data[1] = {0.0}; 
  mat_t* mat = mat_alloc(1, 1);
  mat->data[0] = data[0];

  /* test */

  TEST_ASSERT_EQUAL_FLOAT_ARRAY(data, mat->data, 1);
  TEST_ASSERT_EQUAL_UINT(1, mat->num_rows);
  TEST_ASSERT_EQUAL_UINT(1, mat->num_cols);

  /* free */

  free(mat->data);
  free(mat);
}

/* 3 by 4 */

void check_mat_alloc_3_by_4(void) {

  /* declaration */

  float data[3 * 4] = {
    0.0, 1.0, 2.0, 3.0,
    4.0, 5.0, 6.0, 7.0,
    8.0, 9.0, 10.0, 11.0}; 
  mat_t* mat = mat_alloc(3, 4);
  for (int i = 0; i < 3 * 4; i++) {
    mat->data[i] = data[i];
  }

  /* test */

  TEST_ASSERT_EQUAL_FLOAT_ARRAY(data, mat->data, 3 * 4);
  TEST_ASSERT_EQUAL_UINT(3, mat->num_rows);
  TEST_ASSERT_EQUAL_UINT(4, mat->num_cols);

  /* free */

  free(mat->data);
  free(mat);
}


/**************************************************
 *                                                *
 *              check fml_mat_new                 *
 *                                                *
 **************************************************/

 /* 1 by 1 */

void check_mat_new_1_by_1(void) {

  /* declaration */

  float data[1] = {0.0};
  mat_t* mat = mat_new(1, 1, data);

  /* test */

  TEST_ASSERT_EQUAL_FLOAT_ARRAY(data, mat->data, 1);
  TEST_ASSERT_EQUAL_UINT(1, mat->num_rows);
  TEST_ASSERT_EQUAL_UINT(1, mat->num_cols);  

  /* free */

  free(mat->data);
  free(mat);
}

/* 3 by 4 */

void check_mat_new_3_by_4(void) {

  /* declaration */

  float data[3 * 4] = {
    0.0, 1.0, 2.0, 3.0,
    4.0, 5.0, 6.0, 7.0,
    8.0, 9.0, 10.0, 11.0};
  mat_t* mat = mat_new(3, 4, data);

  /* test */

  TEST_ASSERT_EQUAL_FLOAT_ARRAY(data, mat->data, 1);
  TEST_ASSERT_EQUAL_UINT(3, mat->num_rows);
  TEST_ASSERT_EQUAL_UINT(4, mat->num_cols);

  /* free */

  free(mat->data);
  free(mat);
}

/**************************************************
 *                                                *
 *               check fml_mat_free               *
 *                                                *
 **************************************************/

 /* 1 by 1 */

void check_mat_free_1_by_1(void) {
  
  /* declaration */

  float data[1] = {0.0};
  mat_t* mat_empty = mat_alloc(1, 1);
  mat_t* mat_full = mat_new(1, 1, data);
  
  /* test (using memcheck) */

  mat_free(mat_empty);
  mat_free(mat_full);
}

/* 3 by 4 */

void check_mat_free_3_by_4(void) {
  
  /* declaration */

  float data[12] = {
    0.0, 1.0, 2.0, 3.0,
    4.0, 5.0, 6.0, 7.0,
    8.0, 9.0, 10.0, 11.0};
  mat_t* mat_empty = mat_alloc(3, 4);
  mat_t* mat_full = mat_new(3, 4, data);
  
  /* test (using memcheck) */

  mat_free(mat_empty);
  mat_free(mat_full);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(check_mat_alloc_1_by_1);
  RUN_TEST(check_mat_alloc_3_by_4);
  RUN_TEST(check_mat_new_1_by_1);
  RUN_TEST(check_mat_new_3_by_4);
  check_mat_free_1_by_1();
  check_mat_free_3_by_4();
  return UNITY_END();
}
