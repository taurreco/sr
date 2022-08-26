
#include <stdio.h>
#include "unity.h"
#include "sr.h"
#include "sr_obj.c"
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

    float* pts;
    int* indices;
    int n_pts, n_attr, n_indices;
    
    sr_load_obj(&pts, &indices, &n_pts, &n_attr, &n_indices, "cube.obj");

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("took %f seconds\n", cpu_time_used);
        
        for (int i = 0; i <  n_pts * 8; i++) {
            if (i % 8 == 0) {
                printf("\n");
            }
           printf("%f ", pts[i]);
        }
        
        for (int i = 0; i < n_indices; i++) {
            if (i % 3 == 0) {
              printf("\n");
            }
            printf("%d ", indices[i]);
        }

        free(indices);
        free(pts);
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

