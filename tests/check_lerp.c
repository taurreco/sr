
#include "unity.h"
#include "clip.c"

#include <stdlib.h>
#include <string.h>

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
 *                             base case                             *
 *                                                                   *
 *********************************************************************/


/************
 * same_val *
 ************/

/* interpolate halfway between two scalar values. */

void 
same_val() 
{
    float v0[4] = {
        2, 0, 0, 0
    };

    float v1[4] = {
        2, 0, 0, 0
    };

    float ans[4] = {
        2, 0, 0, 0
    };

    float dest[4];
    float a = 1;

    lerp(dest, v0, v1, a, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 1);
}



/*********************************************************************
 *                                                                   *
 *                       number of attributes                        *
 *                                                                   *
 *********************************************************************/

/************
 * one_attr *
 ************/

/* interpolate halfway between two scalar values. */

void 
one_attr() 
{
    float v0[4] = {
        1, 0, 0, 0
    };

    float v1[4] = {
        2, 0, 0, 0
    };

    float ans[4] = {
        1.5, 0, 0, 0
    };

    float dest[4];
    float a = 0.5;

    lerp(dest, v0, v1, a, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 1);
}

/************
 * two_attr *
 ************/

/* interpolate halfway between two two-dim points. */

void
two_attr()
{
    float v0[4] = {
        3, 1, 0, 0
    };

    float v1[4] = {
        1, 5, 0, 0
    };

    float ans[4] = {
        2, 3, 0, 0
    };

    float dest[4];
    float a = 0.5;

    lerp(dest, v0, v1, a, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4);
}

/************
 * five_attr *
 ************/

/* interpolate halfway between two five-dim points. */

void
five_attr()
{
    float v0[5] = {
        0, 12, 9, 10, 3
    };

    float v1[5] = {
        1, 5, 122, 1, 1
    };

    float ans[5] = {
        0.5, 8.5, 65.5, 5.5, 2
    };

    float dest[5];
    float a = 0.5;

    lerp(dest, v0, v1, a, 5);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 5);
}

/*********************************************************************
 *                                                                   *
 *                  floating point interpolation                     *
 *                                                                   *
 *********************************************************************/

/*****************
 * three_fourths *
 *****************/

/* interpolate three fourths of the way. */

void 
three_fourths() 
{
    float v0[4] = {
        1, 2, 3, 4
    };

    float v1[4] = {
        2, 10, 8, 11
    };

    float ans[4] = {
        1.75, 8, 6.75, 9.25
    };

    float dest[4];
    float a = 0.75;

    lerp(dest, v0, v1, a, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4);
}

/*****************
 * one_sixteenth *
 *****************/

/* interpolate one sixteenth of the way. */

void 
one_sixteenth() 
{
    float v0[4] = {
        1, 2, 3, 4
    };

    float v1[4] = {
        2, 10, 8, 11
    };

    float ans[4] = {
        1.0625, 2.5, 3.3125, 4.4375
    };

    float dest[4];
    float a = 0.0625;

    lerp(dest, v0, v1, a, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4);
}



/*********************************************************************
 *                                                                   *
 *                         complicated cases                         *
 *                                                                   *
 *********************************************************************/

/*************************
 * negative_interpolants *
 *************************/

/* interpolate negative attributes. */

void 
negative_interpolants() 
{
    float v0[4] = {
        1, 2, -3, 4
    };

    float v1[4] = {
        -2, 10, -8, -11
    };

    float ans[4] = {
        -1.25, 8, -6.75, -7.25
    };

    float dest[4];
    float a = 0.75;

    lerp(dest, v0, v1, a, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4);
}

/*******************************
 * floating_point_interpolants *
 *******************************/

/* interpolate floating point attributes. */

void 
floating_point_interpolants() 
{
    float v0[4] = {
        1.2, 2.12, 0.31, 182.98
    };

    float v1[4] = {
        2.01, 10.8, 8.3, 11.1
    };

    float ans[4] = {
        1.4592, 4.8976, 2.8668, 127.9784
    };

    float dest[4];
    float a = 0.32;

    lerp(dest, v0, v1, a, 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(dest, ans, 4);
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
    RUN_TEST(same_val);
    RUN_TEST(one_attr);
    RUN_TEST(two_attr);
    RUN_TEST(five_attr);
    RUN_TEST(three_fourths);
    RUN_TEST(one_sixteenth);
    RUN_TEST(negative_interpolants);
    RUN_TEST(floating_point_interpolants);
    return UNITY_END();
}

