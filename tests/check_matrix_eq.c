#include <stdlib.h>
#include "unity.h"
#include "matrix.h"

void setUp() {}

void tearDown() {}

/*******
 * 1x1 *
 *******/

void check_1x1() {

  /* declaration */
 
  float data[1] = {1.0};
  float data_diff[1] = {2.0};
  mat_t* mat = mat_new(1, 1, data);
  mat_t* eq = mat_new(1, 1, data);
  mat_t* diff = mat_new(1, 1, data_diff);

  /* test */

  TEST_ASSERT_TRUE(mat_eq(mat, eq));
  TEST_ASSERT_FALSE(mat_eq(mat, diff));

  /* free */

  mat_free(mat);
  mat_free(eq);
  mat_free(diff);
}


/*******
 * 2x3 *
 *******/

void check_2x3() {

  /* declaration */
 
  float data[2 * 3] = {
    1.0, 2.0, 3.0,
    4.0, 6.0, 10.0};

  float data_diff[2 * 3] = {
    2.0, 2.0, 3.0,
    4.0, 6.0, 10.0};

  mat_t* mat = mat_new(2, 3, data);
  mat_t* eq = mat_new(2, 3, data);
  mat_t* diff = mat_new(2, 3, data_diff);
  mat_t* diff_dim = mat_alloc(2, 4);
  mat_memset(diff_dim, 10);

  /* test */

  TEST_ASSERT_TRUE(mat_eq(mat, eq));
  TEST_ASSERT_FALSE(mat_eq(mat, diff));
  TEST_ASSERT_FALSE(mat_eq(mat, diff_dim));

  /* free */

  mat_free(mat);
  mat_free(eq);
  mat_free(diff);
  mat_free(diff_dim);
}


int main() {
  UNITY_BEGIN();
  RUN_TEST(check_1x1);
  RUN_TEST(check_2x3);
  return UNITY_END();
}



