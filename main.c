#include <SDL.h>
#include <demos/widgets/lv_demo_widgets.h>
#include <examples/widgets/lv_example_widgets.h>
#include "config.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


#include "stdbool.h"
#include "lv_port_disp.h"


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
}

void update_screen(uint8_t *buf) {
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    Uint8 *pixels = (Uint8 *) (surface->pixels);

// do frame
    memcpy(pixels, buf, SCREEN_WIDTH * SCREEN_HEIGHT * 2);

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void sdl_simple_init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE,
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

Uint32 lvglHeartbeat(Uint32 interval, void *param) {
    lv_tick_inc(1);
    return interval;
}

Uint32 start_time;
const int TARGET_FPS = 60;
const int FRAME_TIME = 1000 / TARGET_FPS;

int main(int argc, char *argv[]) {
    sdl_simple_init();
    SDL_AddTimer(1, lvglHeartbeat, NULL);
    lv_init();
    lv_port_disp_init();
//    lv_demo_stress();
//    lv_demo_benchmark();
//    lv_demo_widgets();
    lv_demo_music();


#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#endif

#ifndef __EMSCRIPTEN__
    // repeatedly calling mainloop on desktop
    while (!quit) {
        start_time = SDL_GetTicks();
        lv_timer_handler();
        loop();
        Uint32 frame_time = SDL_GetTicks() - start_time;
        if (frame_time < FRAME_TIME)
            SDL_Delay(FRAME_TIME - frame_time);
    };
#endif

}