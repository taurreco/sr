#include <stdlib.h>
#include "unity.h"
#include "matrix.h"

void setUp() {}

void tearDown() {}

/******************
 * copy empty 1x1 *
 ******************/

void check_1x1() {

  /* declaration */
 
  mat_t* mat = mat_alloc(1, 1);
  mat_t* cpy = mat_memcpy(mat);

  /* test */

  *mat_at_r(0, 0, mat) = 1.0;
  TEST_ASSERT_EQUAL_UINT(mat->num_rows, cpy->num_rows);
  TEST_ASSERT_EQUAL_UINT(mat->num_cols, cpy->num_cols);
  TEST_ASSERT_NOT_EQUAL(mat_at(0, 0, mat), mat_at(0, 0, cpy));

  /* free */

  mat_free(mat);
  mat_free(cpy);
}


/********************
 * copy full 10x10 *
 ********************/

void check_10x10() {

  /* declaration */
 
  mat_t* mat = mat_alloc(10, 10);
  mat_memset(mat, 1);
  mat_t* cpy = mat_memcpy(mat);
  
  /* test */

  *mat_at_r(0, 0, cpy) = 2.0;
  TEST_ASSERT_EQUAL_UINT(mat->num_rows, cpy->num_rows);
  TEST_ASSERT_EQUAL_UINT(mat->num_cols, cpy->num_cols);
  TEST_ASSERT_EQUAL_FLOAT_ARRAY(mat->data + 1, cpy->data + 1, (10 * 10) - 1);
  TEST_ASSERT_EQUAL_FLOAT(2.0, mat_at(0, 0, cpy));
  TEST_ASSERT_EQUAL_FLOAT(1.0, mat_at(0, 0, mat));
  

  /* free */

  mat_free(mat);
  mat_free(cpy);
}


int main() {
  UNITY_BEGIN();
  RUN_TEST(check_1x1);
  RUN_TEST(check_10x10);
  return UNITY_END();
}


