
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_raster.h"

/*********************************************************************
 *                                                                   *
 *                          unity helpers                            *
 *                                                                   *
 *********************************************************************/

void 
setUp()
{
    /* empty */
}

void 
tearDown() 
{
    /* empty */
}

/*********************************************************************
 *                                                                   *
 *                              tests                                *
 *                                                                   *
 *********************************************************************/

/**************
 * check_swap *
 **************/

/* swap two points */

void
check_swap()
{
    float v0_arr[3] = {0.4, -1.2, 0.2};
    float v1_arr[3] = {4.0, -12.0, 2.0};

    float* v0 = (float*)v0_arr;
    float* v1 = (float*)v1_arr;

    float old_v0[3] = {0.4, -1.2, 0.2};
    float old_v1[3] = {4.0, -12.0, 2.0};

    swap(&v0, &v1);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v0, v1, 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(old_v1, v0, 3);
}

/*********************************************************************
 *                                                                   *
 *                              main                                 *
 *                                                                   *
 *********************************************************************/

int 
main() 
{
    UNITY_BEGIN();
    RUN_TEST(check_swap);
    return UNITY_END();
}
