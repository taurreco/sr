
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "sr_render.h"

/*********************************************************************
 *                                                                   *
 *                           unity helpers                           *
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
 *                            basic tests                            *
 *                                                                   *
 *********************************************************************/

/*************
 * one_point *
 *************/

/* renders exactly one point */
void 
one_point() 
{

}

/****************
 * three_points *
 ****************/

/* renders three points */
void 
three_points() 
{

}


/****************
 * one_triangle *
 ****************/

/* renders exactly one triangle */
void 
one_triangle() 
{

}

/*******************
 * three_triangles *
 *******************/

/* renders three triangles */
void 
three_triangles() 
{

}

/*********************************************************************
 *                                                                   *
 *                         basic vertex shader                       *
 *                                                                   *
 *********************************************************************/

/*******************
 * translate_point *
 *******************/

/* output point is a translation of input */
void 
translate_point() 
{

}

/**********************
 * translate_triangle *
 **********************/

/* output triangle is a translation of input */
void 
translate_triangle() 
{

}


/*********************************************************************
 *                                                                   *
 *                             main                                  *
 *                                                                   *
 *********************************************************************/

int 
main() 
{
    UNITY_BEGIN();
    RUN_TEST(one_point);
    return UNITY_END();
}

