
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "sr.h"

int WIDTH = 1024;
int HEIGHT = 1024;

int main(int argc, char *argv[]) {

    float* depths = calloc(WIDTH * HEIGHT, sizeof(float));
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        depths[i] = 1000;

    uint32_t* colors = calloc(WIDTH * HEIGHT, sizeof(uint32_t));

    float light_color_1[3] = {
        1, 1, 1
    };

    float light_color_2[3] = {
        0.9, 0, 0.3
    };

    float light_pos_1[3] = {
        0, 0, -4
    };

    float light_pos_2[3] = {
        0, 4, 0
    };

    float light_attn_const = 1;
    float light_attn_lin = 2.5;
    float light_attn_quad = 0.4;

    float light_ambient = 3;
    float light_diffuse = 3;
    float light_specular = 0.5;

    sr_light(SR_LIGHT_1, SR_POSITION, light_pos_1);
    sr_light(SR_LIGHT_1, SR_COLOR, light_color_1);

    sr_light(SR_LIGHT_1, SR_AMBIENT, &light_ambient);
    sr_light(SR_LIGHT_1, SR_DIFFUSE, &light_diffuse);
    sr_light(SR_LIGHT_1, SR_SPECULAR, &light_specular);

    sr_light(SR_LIGHT_1, SR_CONSTANT_ATTENUATION, &light_attn_const);
    sr_light(SR_LIGHT_1, SR_LINEAR_ATTENUATION, &light_attn_lin);
    sr_light(SR_LIGHT_1, SR_QUADRATIC_ATTENUATION, &light_attn_quad);

    sr_light(SR_LIGHT_2, SR_POSITION, light_pos_2);
    sr_light(SR_LIGHT_2, SR_COLOR, light_color_2);

    sr_light(SR_LIGHT_2, SR_AMBIENT, &light_ambient);
    sr_light(SR_LIGHT_2, SR_DIFFUSE, &light_diffuse);
    sr_light(SR_LIGHT_2, SR_SPECULAR, &light_specular);

    sr_light(SR_LIGHT_2, SR_CONSTANT_ATTENUATION, &light_attn_const);
    sr_light(SR_LIGHT_2, SR_LINEAR_ATTENUATION, &light_attn_lin);
    sr_light(SR_LIGHT_2, SR_QUADRATIC_ATTENUATION, &light_attn_quad);

    struct sr_obj* obj = sr_load_obj("./assets/stanford-dragon.obj");

    sr_bind_pts(obj->pts, obj->n_pts, obj->n_attr);
    sr_bind_framebuffer(WIDTH, HEIGHT, colors, depths);
    sr_bind_base_color(0.4, 0.8, 0.2);
    sr_bind_phong_vs();
    sr_bind_phong_color_fs();
    sr_matrix_mode(SR_PROJECTION_MATRIX);
    sr_perspective(1, 1, 2, 1000);
    sr_matrix_mode(SR_VIEW_MATRIX);
    sr_look_at(0, 0, 4, 0, 0, 0, 0, 1, 0);
    sr_matrix_mode(SR_MODEL_MATRIX);
    sr_scale(0.1, 0.1, 0.1);
    sr_renderl(obj->indices, obj->n_indices, SR_TRIANGLE_LIST);

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
    
    int cur_time = 0;
    int prev_time = 0;
    int frame = 0;
        
    float theta = 0;
    float phi = 0;

    while (1) {
        
        frame++;

        SDL_Event event;

        while ((SDL_PollEvent(&event)) != 0) {
            if (event.type == SDL_QUIT) { 
                sr_obj_free(obj);
                free(depths);
                free(colors);
                return 0;
            }
        }

        const uint8_t* keystate = SDL_GetKeyboardState(NULL);

         if (keystate[SDL_SCANCODE_RIGHT])
            theta += 0.05;
        
        if (keystate[SDL_SCANCODE_LEFT])
            theta -= 0.05;
        
        if (keystate[SDL_SCANCODE_UP])
            phi += 0.05;
        
        if (keystate[SDL_SCANCODE_DOWN])
            phi -= 0.05;
        
        if (keystate[SDL_SCANCODE_Q]) {
            sr_obj_free(obj);
            free(depths);
            free(colors);
            return 0;
        }

        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            depths[i] = 1000;
            colors[i] = 0;
        }

        light_pos_1[0] = sin(phi) * cos(theta) * 5;
        light_pos_1[1] = cos(phi) * 5;
        light_pos_1[2] = sin(phi) * sin(theta) * 5;

        sr_light(SR_LIGHT_1, SR_POSITION, light_pos_1);
        
        sr_matrix_mode(SR_MODEL_MATRIX);
        sr_renderl(obj->indices, obj->n_indices, SR_TRIANGLE_LIST);

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