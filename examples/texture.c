
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "sr.h"
/*

blue
int WIDTH = 16;
int HEIGHT = 16;

amongus
int WIDTH = 228;
int HEIGHT = 221;

cube tex
int WIDTH = 599;
int HEIGHT = 451;

earth
int WIDTH = 512;
int HEIGHT = 512;

shuttle
int WIDTH = 640;
int HEIGHT = 480;

fern
int WIDTH = 400;
int HEIGHT = 600;

footbal
int WIDTH = 350;
int HEIGHT = 350;

*/



int WIDTH = 228;
int HEIGHT = 221;


int main(int argc, char *argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Rect screenRect = {0, 0, WIDTH, HEIGHT};

    SDL_Window* window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, 
                         SDL_WINDOWPOS_UNDEFINED, screenRect.w, screenRect.h, 
                         SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
                           SDL_TEXTUREACCESS_STREAMING, screenRect.w, screenRect.h);

    uint32_t* tex;
    int height, width;
    sr_load_tga(&tex, &width, &height, "./assets/amongus.tga");

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

        int p;
        uint32_t* pixels;
        SDL_LockTexture(texture, &screenRect, (void**)&pixels, &p);
        uint32_t startTicks = SDL_GetTicks();
       
       
        memcpy(pixels, tex, screenRect.h * screenRect.w * sizeof(uint32_t));

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