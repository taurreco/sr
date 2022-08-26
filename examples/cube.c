
// This work (SDLblit.cpp, by Cory Bloyd) is free of known copyright restrictions.
// https://creativecommons.org/publicdomain/zero/1.0/
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "sr.h"


uint32_t argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b) { 
        return (a<<24) | (r << 16) | (g << 8) | (b << 0); 
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Rect screenRect = {0, 0, 1024, 1024};
    SDL_Window* window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenRect.w, screenRect.h, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenRect.w, screenRect.h);

    float* pts;
    int* indices;
    int n_pts, n_attr, n_indices;
    sr_load_obj(&pts, &indices, &n_pts, &n_attr, &n_indices, "assets/cube.obj");
    sr_bind_pts(pts, n_pts, n_attr);

    float* depths = calloc(1024 * 1024, sizeof(float));
    for (int i = 0; i < 1024 * 1024; i++)
    {
        depths[i] = -1000;
    }
    uint32_t* colors = calloc(1024 * 1024, sizeof(uint32_t));

    sr_bind_framebuffer(1024, 1024, colors, depths);
    sr_bind_vs_test();
    sr_bind_fs_test();
    sr_matrix_mode(SR_PROJECTION_MATRIX);
    sr_perspective(1, 1, 1, 1000);
    sr_matrix_mode(SR_VIEW_MATRIX);
    sr_look_at(0, 0, 10, 0, 0, 0, 0, 1, 0);
    sr_matrix_mode(SR_MODEL_MATRIX);
  // sr_translate(0, 0, -10.3);
  //sr_rotate_x(1);
    sr_scale(1, 1, 1);


    sr_fixed_draw(indices, n_indices, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);

    
        
    for (int frame = 0; ; ++frame) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
            if (event.type == SDL_QUIT)
                return 0;
    
        int pitch;
        uint32_t *pixels;
        SDL_LockTexture(texture, &screenRect, (void**)&pixels, &pitch);
        uint32_t startTicks = SDL_GetTicks();
        for (int y = 0; y < screenRect.h; y++) {
            for (int x = 0; x < screenRect.w; x++) {
                
                pixels[y*screenRect.w + x] = colors[y*screenRect.w + x];
            }
        }

        
        
     for (int i = 0; i < 1024 * 1024; i++)
    {
        depths[i] = -1000;
        colors[i] = 0;
    }   
        sr_rotate_x(0.01);
        sr_rotate_z(0.01);
        sr_matrix_mode(SR_MODEL_MATRIX);    

         
        // sr_rotate_x(0.01);

         sr_fixed_draw(indices, n_indices, SR_PRIMITIVE_TYPE_TRIANGLE_LIST);
    
           
        uint32_t endTicks = SDL_GetTicks();
        SDL_UnlockTexture(texture);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, &screenRect, &screenRect);
        SDL_RenderPresent(renderer);

        char title[32];
        SDL_SetWindowTitle(window, SDL_itoa(endTicks - startTicks, title, 10));
    }
}