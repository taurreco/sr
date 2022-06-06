


#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "rasterize.h"

void setUp() {}

void tearDown() {}

/**************
 * check_swap *
 **************/

void check_swap() {

  float* p1 = (float*)malloc(3 * sizeof(float));
  float* p2 = (float*)malloc(3 * sizeof(float));

  p1[0] = 0.4;
  p1[1] = -1.2;
  p1[2] = 0.2;

  p2[0] = 4.0;
  p2[1] = -12.0;
  p2[2] = 2.0;


  float* old_p1 = (float*)malloc(3 * sizeof(float));
  float* old_p2 = (float*)malloc(3 * sizeof(float));
  
  /*
  float* old_p1_addr = p1;
  float* old_p2_addr = p2;
  */

  /* test values change */

  memcpy(old_p1, p1, sizeof(float) * 3);
  memcpy(old_p2, p2, sizeof(float) * 3);

  TEST_ASSERT_EQUAL_FLOAT_ARRAY(p1, old_p1, 3);
  TEST_ASSERT_EQUAL_FLOAT_ARRAY(p2, old_p2, 3);

  swap_points(&p1, &p2);

  TEST_ASSERT_EQUAL_FLOAT_ARRAY(p2, old_p1, 3);
  TEST_ASSERT_EQUAL_FLOAT_ARRAY(p1, old_p2, 3);

  free(p1);
  free(p2);
  free(old_p1);
  free(old_p2);
  
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(check_swap);
  return UNITY_END();
}
