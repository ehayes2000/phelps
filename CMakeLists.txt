cmake_minimum_required(VERSION 3.5)
project(phelps)
set(CMAKE_CXX_STANDARD 20)


set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)
set(IMGUI_SOURCES 
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
    ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_PATH}/backends/imgui_impl_sdlrenderer2.cpp
)

file(GLOB PROJECT_SOURCES src/*.cpp src/draw/sdl/*.cpp src/fluid/*.cpp)
set(SOURCES ${PROJECT_SOURCES} ${IMGUI_SOURCES})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    ${IMGUI_PATH}
    ${IMGUI_PATH}/backends/
)

target_compile_options(${PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:GCC>:/W4 /WX>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<NOT:$<C_COMPILER_ID:GCC>>:-Wall -Wextra -pedantic -Werror -O3>)
INCLUDE(FindPkgConfig)
find_package(SDL2 REQUIRED)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".mjs")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
    -s USE_ES6_IMPORT_META=0 \
    -s ALLOW_MEMORY_GROWTH=1\
    -s USE_SDL=2 \
    -s WASM=1 \
    -s MODULARIZE=1 \
    -s EXPORT_NAME=Phelps \
    -s EXPORT_ES6 \
    -s ENVIRONMENT=web \
    -s SINGLE_FILE=1")
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC 
  SDL2::Main
)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC 
        "-framework Cocoa" 
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
