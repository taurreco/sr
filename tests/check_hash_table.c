
#include <stdio.h>
#include "unity.h"
#include "sr_load_obj.c"

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
allocate() 
{
    struct hash_table* ht = hash_table_alloc();
    hash_table_free(ht);
}

void
check_hash()
{
    printf("hash of 1234 is %d \n", double_hash("1234", 53, 1));
    printf("hash of 92851345 is %d \n", double_hash("92851345", 53, 1));
}

void
check_insert()
{
    struct hash_table* ht = hash_table_alloc();
    insert(ht, "1/2/3", 0);
    insert(ht, "9/7/3", 1);
    printf("searching for 1/2/3 is %d \n", search(ht, "1/2/3"));
    printf("searching for 9/7/3 is %d \n", search(ht, "9/7/3"));
    printf("searching for 9/7/4 is %d \n", search(ht, "9/7/4"));
    hash_table_free(ht);
}

void
check_grow()
{
    struct hash_table* ht = hash_table_alloc();
    printf("inserting 1/2/3...\n");
    insert(ht, "1/2/3", 0);
    printf("searching for 1/2/3 is %d \n", search(ht, "1/2/3"));
    printf("searching for 9/7/3 is %d \n", search(ht, "9/7/3"));
    printf("growing...\n");
    printf("old size is %d \n", ht->size);
    grow(ht);
    printf("new size is %d \n", ht->size);
    printf("inserting 9/7/3...\n");
    insert(ht, "9/7/3", 1);
    printf("searching for 9/7/3 is %d \n", search(ht, "9/7/3"));
    printf("searching for 9/7/4 is %d \n", search(ht, "9/7/4"));
    hash_table_free(ht);
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
    RUN_TEST(allocate);
    RUN_TEST(check_hash);
    RUN_TEST(check_insert);
    RUN_TEST(check_grow);
    return UNITY_END();
}

