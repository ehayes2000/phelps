#include "draw/sdl/sdlFluid.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


void mainloop(void* ctx)  { 
    SdlFluid* fluid = static_cast<SdlFluid*>(ctx);
    fluid->stepRenderLoop();
    if (fluid->isDone()) { 
        fluid->cleanupRenderLoop();
    }
}

int main() { 
    FluidParameters params;
    SdlFluid *fluid = new SdlFluid(params);
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(mainloop, static_cast<void*>(fluid), 0, 1);
    #else
        fluid->startRenderLoop();
    #endif
}