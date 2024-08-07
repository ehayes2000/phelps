#include "draw/sfmlFluid.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    SfmlFluid fluid(FluidParameters{.renderWidth=720, .renderHeight=480});
    fluid.startRenderLoop();
    return 0;
}
