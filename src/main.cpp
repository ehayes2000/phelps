#include "draw/sdl/sdlFluid.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

uint32_t get_address() {
    return (uint32_t)&get_address;
}

void mainloop(void* ctx)  { 
    SdlFluid* fluid = static_cast<SdlFluid*>(ctx);
    fluid->stepRenderLoop();
    if (fluid->isDone()) { 
        fluid->cleanupRenderLoop();
    }
}

static bool SDL_INIT = false;

int main() { 
    std::cout << "START " << SDL_INIT << std::endl;
    std::cout << "start at" << get_address() << std::endl;
    std::cout << "eat my weener" << std::endl;
    if (!SDL_INIT){
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            std::cout << "INIT SDL" << std::endl;
            printf("Error: %s\n", SDL_GetError());
            exit(1);
        }
    }
    else { 
        std::cout << "DON'T INIT SDL" << std::endl;
    }
    SDL_INIT=true;
    std::cout << "NEW PROGRAM :)" << std::endl;
    FluidParameters params;
    SdlFluid *fluid = new SdlFluid(params);
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(mainloop, static_cast<void*>(fluid), 0, 1);
    #else
        fluid->startRenderLoop();
    #endif
}