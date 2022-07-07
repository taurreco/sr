
// This work (SDLblit.cpp, by Cory Bloyd) is free of known copyright restrictions.
// https://creativecommons.org/publicdomain/zero/1.0/
#include <SDL2/SDL.h>

uint32_t argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b) { 
        return (a<<24) | (r << 16) | (g << 8) | (b << 0); 
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Rect screenRect = {0, 0, 1024, 1024};
    SDL_Window* window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenRect.w, screenRect.h, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenRect.w, screenRect.h);
        
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
             /* pixels[y*screenRect.w + x] = argb(0, frame>>3, y+frame, x+frame);*/
                if ((x % 100 == 0) && (y % 100 == 0)) {
                    pixels[y*screenRect.w + x] = argb(0, 126, 10, 231);
                } else {
                    pixels[y*screenRect.w + x] = 0;
                }
            }
        }
            
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