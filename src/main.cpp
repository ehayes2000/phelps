#include "draw/sfmlFluid.hpp"
#include <iostream>
#include "fluid/vec.hpp"
#include "fluid/particles.hpp"


int main(int argc, char* argv[]) {
    SfmlFluid fluid(FluidParameters{.renderWidth=1080, .renderHeight=720});
    fluid.startRenderLoop();
    return 0;
}
