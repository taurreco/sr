
#include <stdio.h>
#include "unity.h"
#include "sr_obj.c"

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

/* loads one triangle into memory from basic.obj */

void 
basic() 
{
    struct obj_context* obj = obj_context_alloc("cube.obj");

    printf("num verts is %d\n", obj->n_verts);
    printf("num txcoords is %d\n", obj->n_txcoords);
    printf("num norms is %d\n", obj->n_norms);
    printf("num pts is %d\n", obj->n_pts);
    printf("num faces is %d\n", obj->n_faces);

    obj_context_fill(obj);

    for (int i = 0; i < obj->n_verts; i++) {
        printf("%f ", obj->verts[i * 3]);
        printf("%f ", obj->verts[i * 3 + 1]);
        printf("%f ", obj->verts[i * 3 + 2]);
        printf("\n");
    }

    for (int i = 0; i < obj->n_txcoords; i++) {
        printf("%f ", obj->txcoords[i * 2]);
        printf("%f ", obj->txcoords[i * 2 + 1]);
        printf("\n");
    }

    for (int i = 0; i < obj->n_norms; i++) {
        printf("%f ", obj->norms[i * 3]);
        printf("%f ", obj->norms[i * 3 + 1]);
        printf("%f ", obj->norms[i * 3 + 2]);
        printf("\n");
    }

    for (int i = 0; i < obj->n_pts * 8; i++) {
        if (i % 8 == 0) {
            printf("\n");
        }
        printf("%f ", obj->pts[i]);
    }

    for (int i = 0; i < obj->n_faces * 3; i++) {
        if (i % 3 == 0) {
            printf("\n");
        }
        printf("%d ", obj->idxs[i]);
    }
}

void
load()
{
    struct sr_triangle_list* tr_list = sr_load_model("mac.obj");

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

