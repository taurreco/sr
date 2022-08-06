#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_LINE_SIZE 100
#define PRIME_1 16777619
#define PRIME_2 50943779

const uint32_t SIZES[23] = {
    53, 107, 223, 449, 907, 1823, 3659, 
    7321, 14653, 29311, 58631, 117269, 
    234539, 469099, 938207, 1876417, 
    3752839, 7505681, 15011389, 30022781, 
    60045577, 120091177, 240182359
};

int SIZES_INDEX = 0;  /* current position in the array above */

/*********************************************************************
 *                                                                   *
 *                      private declarations                         *
 *                                                                   *
 *********************************************************************/

/**
 * basic double hash table, only intended to be used
 * as a fast "visited" list-- to check if a vertex has been
 * seen already
 */

struct hash_table {
    char** keys;        /* list of obj indices */
    uint32_t n_keys;    /* running count of keys */
    int size;           /* number of buckets in ht */
};

/* hash table functions */

static struct hash_table* 
hash_table_alloc();

static void
hash_table_free(struct hash_table* ht);

static uint32_t
hash(char* string, uint32_t size, uint32_t prime);

static void
insert(struct hash_table* ht, char* key);

static void 
grow(struct hash_table* ht);

static int
member(struct hash_table* ht, char* key);

/*********************************************************************
 *                                                                   *
 *                     (private) obj parsing                         *
 *                                                                   *
 *********************************************************************/

/**************
 * load_model *
 **************/

/* reads obj file data from path into a vertex and index buffer */
void 
load_model(char* path)
{
    FILE* file;
    file = fopen(path, "r");
    if (file == NULL) return;
    char line[MAX_LINE_SIZE];

    /* first pass */

    while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
        printf("%s \n", line);
    }

    /* second pass */
}

/*********************************************************************
 *                                                                   *
 *                      (private) hash table                         *
 *                                                                   *
 *********************************************************************/

/********************
 * hash_table_alloc *
 ********************/

/* allocates empty hashtable of current size */
static struct hash_table* 
hash_table_alloc()
{
    struct hash_table* ht = malloc(sizeof(struct hash_table));
    ht->size = SIZES[SIZES_INDEX];
    ht->n_keys = 0;
    ht->keys = calloc(ht->size, sizeof(struct item*));
    return ht;
}

/*******************
 * hash_table_free *
 *******************/

/* frees hashtable */
static void
hash_table_free(struct hash_table* ht) 
{
    for (int i = 0; i < ht->size; i++) {
        char* key = ht->keys[i];
        if (key != NULL) {
            free(key);
        }
    }
    free(ht->keys);
    free(ht);
}

/********
 * hash *
 ********/

/**
 * given a prime and the size of a hash table, takes a string 
 * and spits out an index into the hash table
 */
static uint32_t
hash(char* string, uint32_t size, uint32_t prime)
{
    uint64_t hash = 0;
    int len_s = strlen(string);
    for (int i = 0; i < len_s; i++) {
        hash += (uint64_t)pow(prime, len_s = (i + 1)) * string[i];
        hash = hash % size;
    }

    return (uint32_t)hash;
}

/***************
 * double_hash *
 ***************/

/**
 * sends a string through two hash functions, useses them with an 
 * attempt counter to find a determenistic index into the hash table
 */
static uint32_t
double_hash(char* string, uint32_t size, uint32_t attempt)
{
    uint32_t hash_a = hash(string, size, PRIME_1);
    uint32_t hash_b = hash(string, size, PRIME_2);
    return (hash_a + (attempt * (hash_b + 1))) % size;
}

/**********
 * insert *
 **********/

/* inserts key into hash table */
static void
insert(struct hash_table* ht, char* key) 
{
    int load = ht->n_keys * 100 / ht->size;
    if (load > 70) {
        grow(ht);
    }
    char* key_cpy = strdup(key);
    int index = double_hash(key_cpy, ht->size, 0);
    char* cur_key = ht->keys[index];
    int i = 1;
    while (cur_key != NULL) {
        index = double_hash(key_cpy, ht->size, i);
        cur_key = ht->keys[index]; /* collided item */
        i++;
    }
    ht->keys[index] = key_cpy;
    ht->n_keys++;
}

/********
 * grow *
 ********/

/** 
 * nearly doubles the capacity of hashtable
 * it finds new size by indexing into a global sizes array
 */
static void 
grow(struct hash_table* ht)
{
    SIZES_INDEX++;
    struct hash_table* new_ht = hash_table_alloc();
    for (int i = 0; i < ht->size; i++) {
        char* key = ht->keys[i];
        if (key != NULL) {
            insert(new_ht, key);
        }
    }
    ht->size = new_ht->size;
    ht->n_keys = new_ht->n_keys;

    ht->size = SIZES[SIZES_INDEX];
    new_ht->size = SIZES[SIZES_INDEX - 1];

    char** tmp_keys = ht->keys;
    ht->keys = new_ht->keys;
    new_ht->keys = tmp_keys;

    hash_table_free(new_ht);
}

/**********
 * member *
 **********/

/* determines whether key is a member of the hashtable */
static int
member(struct hash_table* ht, char* key)
{
    int index = double_hash(key, ht->size, 0);
    char* curr_key = ht->keys[index];
    int i = 1;
    while (curr_key != NULL) {
        if (strcmp(curr_key, key) == 0) {
            return 1;
        }
        index = double_hash(key, ht->size, i);
        curr_key = ht->keys[index];
        i++;
    }
    return 0;
}