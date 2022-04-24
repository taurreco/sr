#include <stdlib.h>
#include "unity.h"
#include "matrix.h"

void setUp() {}

void tearDown() {}

/***********************
 * 1 by 1 set all to 2 *
 ***********************/

void check_1_by_1_all_2() {

  /* declaration */
 
  mat_t* mat = mat_alloc(1, 1);

  /* test */

  mat_memset(mat, 2.0);
  TEST_ASSERT_EQUAL_FLOAT(2.0, mat_at(0, 0, mat));

  /* free */

  mat_free(mat);
}

/*************************
 * 10 by 10 set all to -3 *
 *************************/

void check_10_by_10_all_n3() {

  /* declaration */
  
  float data[10 * 10];
  for (int i = 0; i < 10 * 10; i++) {
    data[i] = -3.0;
  }
  mat_t* mat = mat_alloc(10, 10);

  /* test */

  mat_memset(mat, -3.0);
  TEST_ASSERT_EQUAL_FLOAT_ARRAY(data, mat->data, 10 * 10);

  /* free */

  mat_free(mat);
}





int main() {
  UNITY_BEGIN();
  RUN_TEST(check_1_by_1_all_2);
  RUN_TEST(check_10_by_10_all_n3);
  return UNITY_END();
}

