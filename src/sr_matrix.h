
#ifndef SR_MAT_H
#define SR_MAT_H

/*********************************************************************
 *                                                                   *
 *                     public data structures                        *
 *                                                                   *
 *********************************************************************/

struct mat {
  	size_t num_rows, num_cols;
  	float* entries;
};

/*********************************************************************
 *                                                                   *
 *                      public declarations                          *
 *                                                                   *
 *********************************************************************/

/* memory */
struct mat* mat_alloc(size_t num_rows, size_t num_cols);
void mat_free(struct mat* mat);
void mat_cpy(struct mat* dest, struct mat* src_a, struct mat* src_b);

/* interface */
void mat_set(struct mat* mat, float val, size_t row, size_t col);
float mat_get(struct mat* mat, size_t row, size_t col);

/* operations */
void mat_mul(struct mat* dest, struct mat* src_a, struct mat* src_b);
void mat_scale(struct mat* mat, float val);

#endif /* _SR_MAT_H_ */
