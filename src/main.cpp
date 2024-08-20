#include "draw/sdl/sdlFluid.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void mainloop(void* ctx)  { 
    SdlFluid* fluid = static_cast<SdlFluid*>(ctx);
    if (fluid->isDone()){
        delete fluid;
        #ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();  /* this should "kill" the app. */
        #else
        exit(0);
        #endif
    }
    fluid->stepRenderLoop();
    if (fluid->isDone()) { 
        fluid->cleanupRenderLoop();
    }
}

static bool SDL_INIT = false;

int main() { 
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_INIT=true;
    FluidParameters params;
    SdlFluid *fluid = new SdlFluid(params);
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(mainloop, static_cast<void*>(fluid), 0, 1);
    #else
        fluid->startRenderLoop();
    #endif
}