#include <stdlib.h>
#include "unity.h"
#include "matrix.h"

void setUp() {}

void tearDown() {}

/************************
 * 1 by 1, index [0, 0] *
 ***********************/

void check_1_by_1_at_0_0() {

  /* declaration */

  float data[1] = {1.0}; 
  mat_t* mat = mat_new(1, 1, data);

  /* test */

  TEST_ASSERT_EQUAL_FLOAT(1.0, mat_at(0, 0, mat));

  /* free */

  mat_free(mat);
}

/************************
 * 2 by 2, index [1, 1] *
 ************************/

void check_2_by_2_at_1_1() {

  /* declaration */

  float data[2 * 2] = {
    0.0, 0.0,
    0.0, 1.0}; 
  mat_t* mat = mat_new(2, 2, data);

  /* test */

  TEST_ASSERT_EQUAL_FLOAT(1.0, mat_at(1, 1, mat));

  /* free */

  mat_free(mat);
}

/*****************
 * 3 by 4, edges *
 *****************/

void check_3_by_4_at_edges() {

  /* declaration */

  float data[3 * 4] = {
    0.0, 6.0, 5.0, 0.0,
    1.0, 0.0, 0.0, 4.0,
    0.0, 2.0, 3.0, 0.0}; 
  mat_t* mat = mat_new(3, 4, data);

  /* test */

  TEST_ASSERT_EQUAL_FLOAT(1.0, mat_at(1, 0, mat));
  TEST_ASSERT_EQUAL_FLOAT(2.0, mat_at(2, 1, mat));
  TEST_ASSERT_EQUAL_FLOAT(3.0, mat_at(2, 2, mat));
  TEST_ASSERT_EQUAL_FLOAT(4.0, mat_at(1, 3, mat));
  TEST_ASSERT_EQUAL_FLOAT(5.0, mat_at(0, 2, mat));
  TEST_ASSERT_EQUAL_FLOAT(6.0, mat_at(0, 1, mat));

  /* free */

  mat_free(mat);
}

/*******************************
 * 1 by 1, index [0, 0] to 2.0 *
 *******************************/

void check_1_by_1_set_at_0_0() {

  /* declaration */

  float data[1] = {1.0}; 
  mat_t* mat = mat_new(1, 1, data);

  /* test */

  TEST_ASSERT_EQUAL_FLOAT(1.0, mat_at(0, 0, mat)); 
  *mat_at_r(0, 0, mat) = 2.0;
  TEST_ASSERT_EQUAL_FLOAT(2.0, mat_at(0, 0, mat));

  /* free */
  mat_free(mat);
}

/*******************************
 * 2 by 2, index [1, 1] to 2.0 *
 ******************************/

void check_2_by_2_set_at_1_1() {

  /* declaration */

  float data[2 * 2] = {
    0.0, 0.0,
    0.0, 1.0}; 
  mat_t* mat = mat_new(2, 2, data);

  /* test */

  TEST_ASSERT_EQUAL_FLOAT(1.0, mat_at(1, 1, mat)); 
  *mat_at_r(1, 1, mat) = 2.0;
  TEST_ASSERT_EQUAL_FLOAT(2.0, mat_at(1, 1, mat));

  /* free */

  mat_free(mat);
}

/**********************************
 * 3 by 4, edges all set negative *
 **********************************/

void check_3_by_4_set_at_edges() {

  /* declaration */

  float data[3 * 4] = {
    0.0, 6.0, 5.0, 0.0,
    1.0, 0.0, 0.0, 4.0,
    0.0, 2.0, 3.0, 0.0}; 
  mat_t* mat = mat_new(3, 4, data);

  /* test */

  TEST_ASSERT_EQUAL_FLOAT(1.0, mat_at(1, 0, mat));
  TEST_ASSERT_EQUAL_FLOAT(2.0, mat_at(2, 1, mat));
  TEST_ASSERT_EQUAL_FLOAT(3.0, mat_at(2, 2, mat));
  TEST_ASSERT_EQUAL_FLOAT(4.0, mat_at(1, 3, mat));
  TEST_ASSERT_EQUAL_FLOAT(5.0, mat_at(0, 2, mat));
  TEST_ASSERT_EQUAL_FLOAT(6.0, mat_at(0, 1, mat));

  *mat_at_r(1, 0, mat) = -1.0;
  *mat_at_r(2, 1, mat) = -2.0;
  *mat_at_r(2, 2, mat) = -3.0;
  *mat_at_r(1, 3, mat) = -4.0;
  *mat_at_r(0, 2, mat) = -5.0;
  *mat_at_r(0, 1, mat) = -6.0;
  
  TEST_ASSERT_EQUAL_FLOAT(-1.0, mat_at(1, 0, mat));
  TEST_ASSERT_EQUAL_FLOAT(-2.0, mat_at(2, 1, mat));
  TEST_ASSERT_EQUAL_FLOAT(-3.0, mat_at(2, 2, mat));
  TEST_ASSERT_EQUAL_FLOAT(-4.0, mat_at(1, 3, mat));
  TEST_ASSERT_EQUAL_FLOAT(-5.0, mat_at(0, 2, mat));
  TEST_ASSERT_EQUAL_FLOAT(-6.0, mat_at(0, 1, mat));

  /* free */

  mat_free(mat);
}



int main() {
  UNITY_BEGIN();
  RUN_TEST(check_1_by_1_at_0_0);
  RUN_TEST(check_2_by_2_at_1_1);
  RUN_TEST(check_3_by_4_at_edges);

  RUN_TEST(check_1_by_1_set_at_0_0);
  RUN_TEST(check_2_by_2_set_at_1_1);
  RUN_TEST(check_3_by_4_set_at_edges);
  return UNITY_END();
}
