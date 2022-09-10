
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "sr.h"
#include "sr_math.h"

int WIDTH = 1024;
int HEIGHT = 1024;

int main(int argc, char *argv[]) {


    float* depths = calloc(WIDTH * HEIGHT, sizeof(float));
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        depths[i] = 1000;

    uint32_t* colors = calloc(WIDTH * HEIGHT, sizeof(uint32_t));

    float* mask_pts;
    int* mask_indices;
    int mask_n_indices, mask_n_pts, mask_n_attr;
    uint32_t* mask_texture;
    int mask_t_width, mask_t_height;

    sr_load_obj(&mask_pts, &mask_indices, 
                &mask_n_pts, &mask_n_attr, 
                &mask_n_indices, "./assets/happy_mask.obj");
    sr_load_tga(&mask_texture, &mask_t_width, &mask_t_height, 
                "./assets/happy_mask.tga");
    
    float* link_pts;
    int* link_indices;
    int link_n_indices, link_n_pts, link_n_attr;
    uint32_t* link_texture;
    int link_t_width, link_t_height;
    

    sr_load_obj(&link_pts, &link_indices, 
                &link_n_pts, &link_n_attr, 
                &link_n_indices, "./assets/link.obj");
    sr_load_tga(&link_texture, &link_t_width, &link_t_height, 
                "./assets/link.tga");

    sr_bind_texture(mask_texture, mask_t_width, mask_t_height);
    sr_bind_framebuffer(WIDTH, HEIGHT, colors, depths);
    sr_bind_texture_vs();
    sr_bind_texture_fs();
    sr_matrix_mode(SR_PROJECTION_MATRIX);
    sr_perspective(1, 1, 2, 1000);
    sr_matrix_mode(SR_VIEW_MATRIX);
    sr_look_at(0, 2, 10, 0, 2, 0, 0, 1, 0);
    sr_matrix_mode(SR_MODEL_MATRIX);
    sr_scale(0.05, 0.05, 0.05);
    sr_bind_pts(mask_pts, mask_n_pts, mask_n_attr);
    sr_renderl(mask_indices, mask_n_indices, SR_TRIANGLE_LIST);



    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Rect rect = {0, 0, WIDTH, HEIGHT};

    SDL_Window* window = SDL_CreateWindow("SDL", 
                         SDL_WINDOWPOS_UNDEFINED, 
                         SDL_WINDOWPOS_UNDEFINED, 
                         rect.w, rect.h, 
                         SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
                             SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture = SDL_CreateTexture(renderer, 
                           SDL_PIXELFORMAT_ARGB8888, 
                           SDL_TEXTUREACCESS_STREAMING, 
                           rect.w, rect.h);
    
    float dt = 0;
    clock_t t;
    int cur_time = 0;
    int prev_time = 0;
    int frame = 0;
    int state = 1;

    while (1) {
        
        t = clock();
        frame++;

        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            depths[i] = 1000;
            colors[i] = 0;
        }

        sr_matrix_mode(SR_MODEL_MATRIX);
        sr_rotate_y(0.01);
        if (state) {
            sr_bind_texture(mask_texture, mask_t_width, mask_t_height);
            sr_bind_pts(mask_pts, mask_n_pts, mask_n_attr);
            sr_renderl(mask_indices, mask_n_indices, SR_TRIANGLE_LIST);
        } else {
            sr_bind_texture(link_texture, link_t_width, link_t_height);
            sr_bind_pts(link_pts, link_n_pts, link_n_attr);
            sr_renderl(link_indices, link_n_indices, SR_TRIANGLE_LIST);
        }
        

        SDL_Event event;

        while ((SDL_PollEvent(&event)) != 0) {
            if (event.type == SDL_QUIT) { 
                free(mask_pts);
                free(mask_texture);
                free(mask_indices);
                return 0;
            }


            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                state = !state;
        }

        const uint8_t* keystate = SDL_GetKeyboardState(NULL);

        int p;
        uint32_t *pixels;
        SDL_LockTexture(texture, &rect, (void**)&pixels, &p);

        memcpy(pixels, colors, rect.h * rect.w * sizeof(uint32_t));

        SDL_UnlockTexture(texture);

         cur_time = SDL_GetTicks();
        if (cur_time > prev_time + 1000) {
            printf("FPS: %d\n", frame);
            frame = 0;
            prev_time = cur_time;
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, &rect, &rect);
        SDL_RenderPresent(renderer);
    }
}