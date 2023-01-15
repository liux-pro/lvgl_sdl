#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


#include "stdbool.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_Event e;
SDL_Texture *texture;
bool quit;


void loop() {
    while (SDL_PollEvent(&e)) {
        // window close event
        if (e.type == SDL_QUIT) {
            quit = true;
            break;
        }
    }


    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    Uint8 *pixels = (Uint8 *) (surface->pixels);

// do frame
    uint8_t a[SCREEN_WIDTH * SCREEN_HEIGHT * 2]={0};
    memcpy(pixels, a, SCREEN_WIDTH * SCREEN_HEIGHT * 2);

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void sdl_simple_init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, SDL_WINDOW_RESIZABLE,
                                &window, &renderer);

    //flag 和 depth 实际上没用，并且它们会在sdl3中被删除
    surface = SDL_CreateRGBSurfaceWithFormat(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_PIXELFORMAT_RGB565);
    if (surface == NULL) {
        SDL_Log("SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
        exit(1);
    }
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                SCREEN_HEIGHT);
}

int main(int argc, char *argv[]) {
    sdl_simple_init();



#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#endif

#ifndef __EMSCRIPTEN__
    // repeatedly calling mainloop on desktop
    while (!quit) {
        loop();
        SDL_Delay(16);
    };
#endif

}