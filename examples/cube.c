
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "sr.h"
#include "sr_math.h"

int WIDTH = 1024;
int HEIGHT = 1024;

/*

float cube_pts[24 * 7] = {

    1, 1, -1, 0, 83, 58, 1,  
    -1, 1, -1, 0, 151, 151, 151,
    -1, 1, 1, 0, 85, 100, 111,
    1, 1, 1, 0, 99, 10, 10,  

    1, -1, 1, 0, 86, 194, 138,
    1, 1, 1, 0, 73, 102, 18,
    -1, 1, 1, 0, 3, 102, 173,
    -1, -1, 1, 0, 18, 38, 200,

    -1, -1, 1, 0, 200, 21, 91,
    -1, 1, 1, 0, 162, 134, 38,
    -1, 1, -1, 0, 18, 28, 3,
    -1, -1, -1, 0, 178, 17, 33,

    -1, -1, -1, 0, 20, 121, 176,
    1, -1, -1, 0, 20, 121, 176,
    1, -1, 1, 0, 20, 121, 176,
    -1, -1, 1, 0, 20, 121, 176,

    1, -1, -1, 0, 43, 29, 164,
    1, 1, -1, 0, 43, 29, 164,
    1, 1, 1, 0, 43, 29, 164,
    1, -1, 1, 0, 43, 29, 164,

    -1, -1, -1, 0, 198, 7, 163,
    -1, 1, -1, 0, 198, 7, 163,
    1, 1, -1, 0, 198, 7, 163,
    1, -1, -1, 0, 198, 7, 163
};

*/


float cube_pts[24 * 5] = {

    1, 1, -1, 0.5, 0.668,  
    -1, 1, -1, 0.5, 0.332,
    -1, 1, 1, 0.25, 0.332,
    1, 1, 1, 0.25, 0.668,

    1, -1, 1, 0, 0.668,
    1, 1, 1, 0.25, 0.668,
    -1, 1, 1, 0.25, 0.332,
    -1, -1, 1, 0, 0.332, 

    -1, -1, 1, 0.25, 0, 
    -1, 1, 1, 0.25, 0.332,
    -1, 1, -1, 0.5, 0.332, 
    -1, -1, -1, 0.5, 0, 

    -1, -1, -1, 1, 0.332,
    1, -1, -1, 0.75, 0.332, 
    1, -1, 1, 0.75, 0.668, 
    -1, -1, 1, 1, 0.668,  

    1, -1, -1, 0.5, 1,
    1, 1, -1, 0.5, 0.668,
    1, 1, 1, 0.25, 0.668,
    1, -1, 1, 0.25, 1,

    -1, -1, -1, 0.75, 0.332,  
    -1, 1, -1, 0.5, 0.332,
    1, 1, -1, 0.5, 0.668, 
    1, -1, -1, 0.75, 0.668, 
};

int cube_indices[12 * 3] = {
    0, 1, 2,
    0, 2, 3,
    4, 5, 6,
    4, 6, 7,
    8, 9, 10,
    8, 10, 11,
    12, 13, 14,
    12, 14, 15,
    16, 17, 18,
    16, 18, 19,
    20, 21, 22,
    20, 22, 23
};

int main(int argc, char *argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Rect screenRect = {0, 0, WIDTH, HEIGHT};

    SDL_Window* window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, 
                         SDL_WINDOWPOS_UNDEFINED, screenRect.w, screenRect.h, 
                         SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
                           SDL_TEXTUREACCESS_STREAMING, screenRect.w, screenRect.h);


    float* depths = calloc(WIDTH * HEIGHT, sizeof(float));
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        depths[i] = -1000;
    }
    uint32_t* colors = calloc(WIDTH * HEIGHT, sizeof(uint32_t));

    uint32_t* cube_texture;
    int t_width, t_height;
    sr_load_tga(&cube_texture, &t_width, &t_height, "./assets/minecraft_dirt.tga");
    sr_bind_texture(cube_texture, t_width, t_height);
    sr_bind_pts(cube_pts, 24, 5);
    sr_bind_framebuffer(WIDTH, HEIGHT, colors, depths);
    sr_bind_texture_vs();
    sr_bind_texture_fs();
    sr_matrix_mode(SR_PROJECTION_MATRIX);
    sr_perspective(1, 1, 2, 1000);
    sr_matrix_mode(SR_VIEW_MATRIX);
    sr_look_at(0, 0, 10, 0, 0, 0, 0, 1, 0);
    sr_matrix_mode(SR_MODEL_MATRIX);
    sr_renderl(cube_indices, 12 * 3, SR_TRIANGLE_LIST);


    float pitch = 0;
    float yaw = -90;

    float f[3] = {
        0, 0, -1
    };

    float l[3] = {
        0, 0, 0
    };

    float u[3] = {
        0, 1, 0
    };

    float pos[3] = {
        0, 0, 10
    };

    float dt = 0;
    clock_t t;
    int cur_time = 0;
    int prev_time = 0;

    for (int frame = 0; ; ++frame) {
        t = clock();
        SDL_Event event;

        while ((SDL_PollEvent(&event)) != 0) {
            /*! request quit */
            if (event.type == SDL_QUIT) { 
                return 0;
            }
        }

        const uint8_t* keystate = SDL_GetKeyboardState(NULL);

        if (keystate[SDL_SCANCODE_W]) {
            pos[0] += f[0] * 10 * dt;
            pos[1] += f[1] * 10 * dt;
            pos[2] += f[2] * 10 * dt;
        }

        if (keystate[SDL_SCANCODE_S]) {
            pos[0] -= f[0] * 10 * dt;
            pos[1] -= f[1] * 10 * dt;
            pos[2] -= f[2] * 10 * dt;
        }

        if (keystate[SDL_SCANCODE_W]) {
            pos[0] += f[0] * 10 * dt;
            pos[1] += f[1] * 10 * dt;
            pos[2] += f[2] * 10 * dt;
        }

        if (keystate[SDL_SCANCODE_A]) {
            pos[0] -= l[0] * 10 * dt;
            pos[1] -= l[1] * 10 * dt;
            pos[2] -= l[2] * 10 * dt;
        }

        if (keystate[SDL_SCANCODE_D]) {
            pos[0] += l[0] * 10 * dt;
            pos[1] += l[1] * 10 * dt;
            pos[2] += l[2] * 10 * dt;
        }

        if (keystate[SDL_SCANCODE_RIGHT])
            yaw += 0.5;
        
        if (keystate[SDL_SCANCODE_LEFT])
            yaw -= 0.5;
        
        if (keystate[SDL_SCANCODE_UP])
            pitch += 0.5;
        
        if (keystate[SDL_SCANCODE_DOWN])
            pitch -= 0.5;
        
        if (keystate[SDL_SCANCODE_Q])
            return 0;

        if (pitch >= 80)
            pitch = 80;

        if (pitch <= -80)
            pitch = -80;     

        f[0] = cos(radians(yaw)) * cos(radians(pitch));
        f[1] = sin(radians(pitch));
        f[2] = sin(radians(yaw)) * cos(radians(pitch));
        normalize(f);
        cross(l, f, u);   /* orthonormal to f and up */
        normalize(l);
        cross(u, l, f);  /* orthonormal up */
        normalize(u);

        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            depths[i] = -1000;
            colors[i] = 0;
        } 

        sr_matrix_mode(SR_VIEW_MATRIX);
        sr_load_identity();
        sr_look_at(pos[0], pos[1], pos[2], 
                   pos[0] + f[0], pos[1] + f[1], pos[2] + f[2], 
                   u[0], u[1], u[2]); 
        sr_matrix_mode(SR_MODEL_MATRIX);
        sr_rotate_x(1 * dt);
        sr_rotate_y(1 * dt);
        sr_renderl(cube_indices, 12 * 3, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);

        int p;
        uint32_t *pixels;
        SDL_LockTexture(texture, &screenRect, (void**)&pixels, &p);
        uint32_t startTicks = SDL_GetTicks();

        memcpy(pixels, colors, screenRect.h * screenRect.w * sizeof(uint32_t));

        uint32_t endTicks = SDL_GetTicks();
        dt = (float)(clock() - t) / CLOCKS_PER_SEC;
        SDL_UnlockTexture(texture);

         cur_time = SDL_GetTicks();
        if (cur_time > prev_time + 1000) {
            printf("FPS: %d\n", frame);
            frame = 0;
            prev_time = cur_time;
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, &screenRect, &screenRect);
        SDL_RenderPresent(renderer);

        char title[32];
        SDL_SetWindowTitle(window, SDL_itoa(endTicks - startTicks, title, 10));
    }
}