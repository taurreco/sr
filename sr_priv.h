
/*********************************************************************
 *                                                                   *
 *                               uniform                             *
 *                                                                   *
 *********************************************************************/

/***********
 * texture *
 ***********/

struct texture {
    uint32_t* data;
    int n_pixels;
};

/*********
 * light *
 *********/

/* holds light data */

struct light {
    enum sr_light_type type;
    float pos[3];
    float color[4];
    float dir[3];
    float spot_angle;
    float spot_penumbra;
    float attn_const;
    float attn_lin;
    float attn_quad;
};

/************
 * material *
 ************/

struct material {
    struct texture texture;
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float blend;
    float shininess;
};

/***********
 * uniform *
 ***********/

/* the uniform variables for the fixed lib shaders */

struct uniform {
    struct mat4* model;             /* geometry */
    struct mat4* normal;
    struct mat4* mvp;
    float cam_pos[3];
    int has_texture;                /* material */
    struct material* material;
    uint8_t light_state;            /* light */
    struct light* lights;
    float ka;
    float kd;
    float ks;
};

/*********************************************************************
 *                                                                   *
 *                             rasterizer                            *
 *                                                                   *
 *********************************************************************/

/**********
 * raster *
 **********/

struct raster {
    struct sr_framebuffer* fbuf;
    void* uniform;
    fs_f fs;
    int n_attr;
    int winding;
};

void draw_pt(struct raster* rast, float* pt);
void draw_ln(struct raster* rast, float* v0, float* v1);
void draw_tr(struct raster* rast, float* v0, float* v1, float* v2);

/*********************************************************************
 *                                                                   *
 *                        clipping functions                         *
 *                                                                   *
 *********************************************************************/

void clip_poly(float* src, int* n_pts, 
               int n_attr, uint8_t clip_flags);
void clip_test(float* pt, uint8_t* flags);

/*********************************************************************
 *                                                                   *
 *                               math                                *
 *                                                                   *
 *********************************************************************/

struct mat4 {
    float e00, e01, e02, e03;
    float e10, e11, e12, e13;
    float e20, e21, e22, e23;
    float e30, e31, e32, e33;
};

void matmul(struct mat4* a, struct mat4* b);
int invert(struct mat4* a);
void transpose(struct mat4* a);
void upper_3x3(struct mat4* a);
void vec4_matmul(float* a, struct mat4* b, float* c);
void vec4_mul(float* a, float* b, float* c);
void vec4_add(float* a, float* b, float* c);
void vec4_scale(float* a, float* b, float c);
void lerp(float* a, float* b, float* c, float alpha);
void vec3_sub(float* a, float* b, float* c);
void vec3_add(float* a, float* b, float* c);
void vec3_scale(float* a, float* b, float c);
void reflect(float* r, float* n, float* v);
float dot(float* a, float* b);
void cross(float* a, float* b, float* c);
float magnitude(float* a);
void normalize(float* a);
float radians(float deg);