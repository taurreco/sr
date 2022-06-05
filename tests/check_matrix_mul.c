#include <stdlib.h>
#include "unity.h"
#include "matrix.h"

void setUp() {}

void tearDown() {}

/***********
 * invalid *
 ***********/

void check_invalid() {

  /* declaration */

  mat_t* mat_l = mat_alloc(3, 2);
  mat_t* mat_r = mat_alloc(10, 1);
  
  mat_memset(mat_l, 1);
  mat_memset(mat_r, 1);
  /* test */

  mat_t* mat_p = mat_mul(mat_l, mat_r);
  TEST_ASSERT_EQUAL_PTR(NULL, mat_p);
  

  /* free */

  mat_free(mat_l);
  mat_free(mat_r);
}


/***********************
 * 1 by 1 times 1 by 1 *
 ***********************/

void check_1x1_times_1x1() {

  /* declaration */

  float data_l[1] = {3.0};
  float data_r[1] = {2.0};
  float data_e[1] = {6.0};

  mat_t* mat_l = mat_new(1, 1, data_l);
  mat_t* mat_r = mat_new(1, 1, data_r);
  mat_t* mat_e = mat_new(1, 1, data_e);

  /* test */

  mat_t* mat_p = mat_mul(mat_l, mat_r);
  TEST_ASSERT_EQUAL_UINT(mat_e->num_rows, mat_p->num_rows);
  TEST_ASSERT_EQUAL_UINT(mat_e->num_cols, mat_p->num_cols);
  TEST_ASSERT_EQUAL_FLOAT_ARRAY(mat_e->data, mat_p->data, 1);
  

  /* free */

  mat_free(mat_l);
  mat_free(mat_r);
  mat_free(mat_e);
  mat_free(mat_p);
}

/***********************
 * 2 by 1 times 1 by 2 *
 ***********************/

void check_2x1_times_1x2() {

  /* declaration */

  float data_l[2] = {3.0, 2.0};
  float data_r[2] = {4.0, 5.0};
  float data_e[2 * 2] = { /* expect a 2 x 2 */
    12.0, 15.0,
    8.0, 10.0};

  mat_t* mat_l = mat_new(2, 1, data_l);
  mat_t* mat_r = mat_new(1, 2, data_r);
  mat_t* mat_e = mat_new(2, 2, data_e);

  /* test */

  mat_t* mat_p = mat_mul(mat_l, mat_r);
  TEST_ASSERT_EQUAL_UINT(mat_e->num_rows, mat_p->num_rows);
  TEST_ASSERT_EQUAL_UINT(mat_e->num_cols, mat_p->num_cols);
  TEST_ASSERT_EQUAL_FLOAT_ARRAY(mat_e->data, mat_p->data, 2 * 2);
  

  /* free */

  mat_free(mat_l);
  mat_free(mat_r);
  mat_free(mat_e);
  mat_free(mat_p);
}

/***********************
 * 3 by 4 times 4 by 2 *
 ***********************/

void check_3x4_times_4x2() {

  /* declaration */

  float data_l[3 * 4] = {
    12.0, 10.0, 9.0, -3.2,
    3.0, 5.0, 110.0, 2.0,
    34.0, 7.0, 3.0, 0.0};
  
  float data_r[4 * 2] = {
    90.0, 3.0,
    12.0, 2.0,
    2.0, 5.0,
    75.0, 0.1};

  float data_e[3 * 2] = { /* expect a 3 x 2 */
    978.0, 100.68,
    700.0, 569.2,
    3150.0, 131.0};

  mat_t* mat_l = mat_new(3, 4, data_l);
  mat_t* mat_r = mat_new(4, 2, data_r);
  mat_t* mat_e = mat_new(3, 2, data_e);

  /* test */

  mat_t* mat_p = mat_mul(mat_l, mat_r);
  TEST_ASSERT_EQUAL_UINT(mat_e->num_rows, mat_p->num_rows);
  TEST_ASSERT_EQUAL_UINT(mat_e->num_cols, mat_p->num_cols);
  TEST_ASSERT_EQUAL_FLOAT_ARRAY(mat_e->data, mat_p->data, 3 * 2);
  

  /* free */

  mat_free(mat_l);
  mat_free(mat_r);
  mat_free(mat_e);
  mat_free(mat_p);
}


int main() {
  UNITY_BEGIN();
  RUN_TEST(check_1x1_times_1x1);
  RUN_TEST(check_invalid);
  RUN_TEST(check_2x1_times_1x2);
  RUN_TEST(check_3x4_times_4x2);
  return UNITY_END();
}

