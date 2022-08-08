#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "sr.h";

#define VT 1 << 0
#define VN 1 << 1

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

/* structs */

struct kv_pair {
    char* key;
    int val;
};

struct hash_table {
    struct kv_pair** pairs;         /* list of obj indices */
    uint32_t n_pairs;               /* running count of keys */
    int size;                       /* number of buckets in ht */
};

struct obj_context {

    FILE* file;

    float* verts;                   /* vertex positions x, y, z */
    float* txcoords;                /* texture coordinates u, v */
    float* norms;                   /* vertex normals nx, ny, nz */
    float* pts;                     /* pts read by sr with all attributes */
    uint32_t* idxs;                 /* indexes into pts to describe faces */

    uint32_t n_verts;               /* sizes of the buffers above */
    uint32_t n_txcoords;
    uint32_t n_norms;
    uint32_t n_pts;
    uint32_t n_faces;

    struct hash_table* visited;
};

/* obj loader */

extern struct sr_triangle_list*
sr_load_model(char* file);

/* obj context memory & parser */

static struct obj_context*
obj_context_alloc(char* file);

static void
obj_context_fill(struct obj_context* obj);

static void
obj_context_free(struct obj_context* obj);

/* parsing helpers */

static void
push_stream(float* dst, char* stream, int n_words);

static void
process_pt(struct obj_context* obj, char flags, char* pt_raw);

static void
split_indices(char* raw, char flags, int* v_idx_p, 
              int* vt_idx_p, int* vn_idx_p);

/* hash table memory */

static struct kv_pair*
kv_pair_alloc(char* key, int val);

static struct hash_table* 
hash_table_alloc();

static void
kv_pair_free(struct kv_pair*);

static void
hash_table_free(struct hash_table* ht);

/* hash table api */

static void
insert(struct hash_table* ht, char* key, int val);

static int
search(struct hash_table* ht, char* key);

/* hash table helpers */

static void 
grow(struct hash_table* ht);

/* hashing */

static uint32_t
double_hash(char* string, uint32_t size, uint32_t attempt);

static uint32_t
hash(char* string, uint32_t size, uint32_t prime);

/*********************************************************************
 *                                                                   *
 *                       (public) obj loading                        *
 *                                                                   *
 *********************************************************************/

/*****************
 * sr_load_model *
 *****************/

/* reads obj file data from path into an indexed triangle list */
extern struct sr_triangle_list*
sr_load_model(char* file)
{
    /* open file and pre allocate buffers in obj context */

    struct obj_context* obj = obj_context_alloc(file);

    /* fill buffers in obj context (readable by sr) */

    obj_context_fill(obj);

    /* allocate a new indexed triangle list */

    struct sr_triangle_list* tr_list;
    tr_list = malloc(sizeof(struct sr_triangle_list));

    /* copy over relevant data from the obj context */

    tr_list->n_attr = 8;
    tr_list->n_pts = obj->n_pts;
    tr_list->n_indices = obj->n_faces * 3;
    tr_list->pts = malloc(tr_list->n_pts * tr_list->n_attr * sizeof(float));
    tr_list->indices = malloc(tr_list->n_indices * sizeof(int));

    memcpy(tr_list->pts, obj->pts, 
           tr_list->n_pts * tr_list->n_attr * sizeof(float));
    memcpy(tr_list->indices, obj->idxs, 
           tr_list->n_indices * sizeof(int));

    /* free data we don't need anymore */

    obj_context_free(obj);

    return tr_list;
}

/*********************************************************************
 *                                                                   *
 *                  (private) obj memory & parsing                   *
 *                                                                   *
 *********************************************************************/

/*********************
 * obj_context_alloc *
 *********************/

/** 
 * reads obj file and allocates as much space as is needed 
 * for obj context fields 
 */
static struct obj_context*
obj_context_alloc(char* file)
{
    char line[MAX_LINE_SIZE];
    char* token;

    struct obj_context* obj = calloc(1, sizeof(struct obj_context));
    obj->file = fopen(file, "r");

    /* first pass */

    obj->visited = hash_table_alloc();

    while (fgets(line, MAX_LINE_SIZE, obj->file) != NULL) {
        token = strtok(line, "\n");
        if (token == NULL)
            continue;
        token = strtok(line, " ");
        if (strcmp(token, "v") == 0) {
            obj->n_verts++;
        }
        if (strcmp(token, "vt") == 0) {
            obj->n_txcoords++;
        }
        if (strcmp(token, "vn") == 0) {
            obj->n_norms++;
        }
        if (strcmp(token, "f") == 0) {
            int i = 0;
            token = strtok(NULL, " ");
            while (token != NULL) {
                if (search(obj->visited, token) == -1) {
                    insert(obj->visited, token, obj->n_pts);
                    obj->n_pts++;
                }
                i++;
                token = strtok(NULL, " ");
            }
            obj->n_faces += 1 + (i - 3);
        }
    }

    fseek(obj->file, 0, SEEK_SET);

    obj->verts = malloc(obj->n_verts * 3 * sizeof(float));
    obj->txcoords = malloc(obj->n_txcoords * 2 * sizeof(float));
    obj->norms = malloc(obj->n_norms * 3 * sizeof(float));
    obj->pts = calloc(obj->n_pts * 8, sizeof(float));
    obj->idxs = malloc(obj->n_faces * 3 * sizeof(int));
}

/********************
 * obj_context_fill *
 ********************/

/**
 * reads obj file and moves data into obj context fields, 
 * structured appropriately
 */
static void
obj_context_fill(struct obj_context* obj) 
{
    int v_idx = 0;
    int vt_idx = 0;
    int vn_idx = 0;
    int tr_idx = 0;

    char line[MAX_LINE_SIZE];
    char* token;

    char flags = 0;

    /* second pass */

    while (fgets(line, MAX_LINE_SIZE, obj->file) != NULL) {
        token = strtok(line, "\n");
        if (token == NULL)
            continue;
        token = strtok(line, " ");
        if (strcmp(token, "v") == 0) {
            char* stream = strtok(NULL, "\n");
            push_stream(obj->verts + v_idx * 3, stream, 3);
            v_idx++;
            continue;
        }
        if (strcmp(token, "vt") == 0) {
            flags |= VT;
            char* stream = strtok(NULL, "\n");
            push_stream(obj->txcoords + vt_idx * 2, stream, 2);
            vt_idx++;
            continue;
        }
        if (strcmp(token, "vn") == 0) {
            flags |= VN;
            char* stream = strtok(NULL, "\n");
            push_stream(obj->norms + vn_idx * 3, stream, 3);
            vn_idx++;
            continue;
        }
        if (strcmp(token, "f") == 0) {
            char* v0_raw = strtok(NULL, " ");
            char* v1_raw = strtok(NULL, " ");
            char* v2_raw = strtok(NULL, " ");
            
            while (v2_raw != NULL) {

                process_pt(obj, flags, v0_raw);
                process_pt(obj, flags, v1_raw);
                process_pt(obj, flags, v2_raw);

                /* copy over indices */
                obj->idxs[tr_idx * 3] = search(obj->visited, v0_raw);
                obj->idxs[tr_idx * 3 + 1] = search(obj->visited, v1_raw);
                obj->idxs[tr_idx * 3 + 2] = search(obj->visited, v2_raw);

                v1_raw = v2_raw;
                v2_raw = strtok(NULL, " ");
                tr_idx++;
            }
        }
    }
}

/********************
 * obj_context_free *
 ********************/

/** 
 * frees all the data within an obj context, 
 * and closes its associated file
 */
static void
obj_context_free(struct obj_context* obj)
{
    fclose(obj->file);
    free(obj->verts);
    free(obj->txcoords);
    free(obj->norms);
    free(obj->pts);
    free(obj->idxs);
    hash_table_free(obj->visited);
    free(obj);
}

/*********************************************************************
 *                                                                   *
 *                   (private) hash table helpers                    *
 *                                                                   *
 *********************************************************************/

/***************
 * push_stream *
 ***************/

/**
 * takes a stream of whitespace seperated strings, converts 
 * them to floats, and copies it to the dst buffer
 */
static void
push_stream(float* dst, char* stream, int n_str) 
{
    char* start = stream;
    char* end;
    for (int i = 0; i < n_str; i++) {
        float val = strtof(start, &end);
        dst[i] = val;
        char* tmp = start;
        start = end;
        end = tmp;
    }
}

/**************
 * process_pt *
 **************/

/**
 * reads raw obj point data and converts it into a point 
 * readable by the sr pipeline, then adds to obj context 
 * point buffer 
 */
static void
process_pt(struct obj_context* obj, char flags, char* pt_raw)
{
    char tmp[256];
    strcpy(tmp, pt_raw);
    int v_idx, vt_idx, vn_idx;
    float* pts = obj->pts + search(obj->visited, pt_raw) * 8;

    split_indices(tmp, flags, &v_idx, &vt_idx, &vn_idx);
    memcpy(pts, obj->verts + v_idx * 3, 3 * sizeof(float));

    if (flags & (VT) ) { /* pos & txcoord */
        memcpy(pts + 3, obj->txcoords + vt_idx * 2, 2 * sizeof(float));
    }

    if (flags & (VN) ) {
        memcpy(pts + 5, obj->norms + vn_idx * 3, 3 * sizeof(float));
    }
}

/*****************
 * split_indices *
 *****************/

/* converts raw obj point indices into readable indices */
static void
split_indices(char* raw, char flags, int* v_idx_p, 
              int* vt_idx_p, int* vn_idx_p)
{
    char* end_idx;
    switch (flags) {
        case 0:
            *v_idx_p = atoi(strtok_r(raw, "/", &end_idx)) - 1;
            break;
        case VT:
            *v_idx_p = atoi(strtok_r(raw, "/", &end_idx)) - 1;
            *vt_idx_p = atoi(strtok_r(NULL, "/", &end_idx)) - 1;
            break;
        case VN:
            *v_idx_p = atoi(strtok_r(raw, "/", &end_idx)) - 1;
            *vn_idx_p = atoi(strtok_r(NULL, "/", &end_idx)) - 1;
            break;
        case VT | VN:
            *v_idx_p = atoi(strtok_r(raw, "/", &end_idx)) - 1;
            *vt_idx_p = atoi(strtok_r(NULL, "/", &end_idx)) - 1;
            *vn_idx_p = atoi(strtok_r(NULL, "/", &end_idx)) - 1;
            break;
    }
}

/*********************************************************************
 *                                                                   *
 *                (private) kv_pair memory management                *
 *                                                                   *
 *********************************************************************/

/*****************
 * kv_pair_alloc *
 *****************/

/* allocates a key value pair */
static struct kv_pair* 
kv_pair_alloc(char* key, int val)
{
    struct kv_pair* pair = malloc(sizeof(struct kv_pair));
    pair->key = strdup(key);
    pair->val = val;
    return pair;
}

/****************
 * kv_pair_free *
 ****************/

/* frees kv_pair */
static void
kv_pair_free(struct kv_pair* pair) 
{
    free(pair->key);
    free(pair);
}

/*********************************************************************
 *                                                                   *
 *             (private) hash table memory management                *
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
    ht->n_pairs = 0;
    ht->pairs = calloc(ht->size, sizeof(struct kv_pair*));
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
        struct kv_pair* pair= ht->pairs[i];
        if (pair != NULL) {
            kv_pair_free(pair);
        }
    }
    free(ht->pairs);
    free(ht);
}

/*********************************************************************
 *                                                                   *
 *             (private) hash table interface functions              *
 *                                                                   *
 *********************************************************************/

/**********
 * insert *
 **********/

/* inserts key into hash table */
static void
insert(struct hash_table* ht, char* key, int val) 
{
    int load = ht->n_pairs * 100 / ht->size;
    if (load > 70) {
        grow(ht);
    }
    struct kv_pair* pair = kv_pair_alloc(key, val);
    int index = double_hash(pair->key, ht->size, 0);
    struct kv_pair* cur_pair = ht->pairs[index];
    int i = 1;
    while (cur_pair != NULL) {
        index = double_hash(pair->key, ht->size, i);
        cur_pair = ht->pairs[index]; /* collided item */
        i++;
    }
    ht->pairs[index] = pair;
    ht->n_pairs++;
}

/**********
 * search *
 **********/

/* returns val of key or -1 if it isn't in the hash table */
static int
search(struct hash_table* ht, char* key)
{
    int index = double_hash(key, ht->size, 0);
    struct kv_pair* cur_pair = ht->pairs[index];
    int i = 1;
    while (cur_pair != NULL) {
        if (strcmp(cur_pair->key, key) == 0) {
            return cur_pair->val;
        }
        index = double_hash(key, ht->size, i);
        cur_pair = ht->pairs[index];
        i++;
    }
    return -1;
}

/*********************************************************************
 *                                                                   *
 *                    (private) hash table helpers                   *
 *                                                                   *
 *********************************************************************/

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
        struct kv_pair* pair = ht->pairs[i];
        if (pair != NULL) {
            insert(new_ht, pair->key, pair->val);
        }
    }
    ht->size = new_ht->size;
    ht->n_pairs = new_ht->n_pairs;

    ht->size = SIZES[SIZES_INDEX];
    new_ht->size = SIZES[SIZES_INDEX - 1];

    /* swap list of kv_pairs */

    struct kv_pair** tmp_pairs = ht->pairs;
    ht->pairs = new_ht->pairs;
    new_ht->pairs = tmp_pairs;

    hash_table_free(new_ht);
}

/*********************************************************************
 *                                                                   *
 *                     (private) string hashing                      *
 *                                                                   *
 *********************************************************************/

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