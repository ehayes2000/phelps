#include "draw/sfmlFluid.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    SfmlFluid fluid(FluidParameters{.renderWidth=1920, .renderHeight=1080});
    fluid.startRenderLoop();
    return 0;
}
