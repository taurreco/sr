
#include <stdio.h>
#include "unity.h"
#include "sr.h"
#include "sr_tga.c"
#include <time.h>

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
 *                             test cases                            *
 *                                                                   *
 *********************************************************************/

/*********
 * basic *
 *********/

void
load()
{
    clock_t start, end;
    double cpu_time_used;
     
    start = clock();

    uint32_t* tex;
    sr_load_tga(&tex, "./assets/football_seal.tga");

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("took %f seconds\n", cpu_time_used);
        
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
    RUN_TEST(load);
    return UNITY_END();
}

