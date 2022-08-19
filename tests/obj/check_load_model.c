
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
    
    struct sr_triangle_list* tr_list = sr_load_model("ogre.obj");

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("took %f seconds\n", cpu_time_used);


    if (tr_list) {

        
        for (int i = 0; i < tr_list->n_pts * 8; i++) {
            if (i % 8 == 0) {
                printf("\n");
            }
           printf("%f ", tr_list->pts[i]);
        }
        /*
        for (int i = 0; i < tr_list->n_indices; i++) {
            if (i % 3 == 0) {
              printf("\n");
            }
            printf("%d ", tr_list->indices[i]);
        }
*/
        free(tr_list->indices);
        free(tr_list->pts);
        free(tr_list);
    }


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

