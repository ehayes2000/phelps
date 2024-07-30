#include "sfmlFluid.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    
    SfmlFluid fluid(720, 480);
    fluid.startRenderLoop();
    return 0;
}