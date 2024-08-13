cmake_minimum_required(VERSION 3.5)
project(phelps)
set(CMAKE_CXX_STANDARD 20)

# Set the path to ImGui
set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

# ImGui source files
set(IMGUI_SOURCES
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
    ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
)

# Your project source files
file(GLOB PROJECT_SOURCES src/*.cpp)

# Combine all sources
set(SOURCES ${PROJECT_SOURCES} ${IMGUI_SOURCES})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    ${IMGUI_PATH}
    ${IMGUI_PATH}/backends
)

target_compile_options(${PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:MSVC>:/W4 /WX>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

# Find required packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC 
    SDL2::Main
    OpenGL::GL
)

# For macOS, we need to link against the Cocoa and IOKit frameworks
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC 
        "-framework Cocoa" 
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()


# cmake_minimum_required(VERSION 3.5)
# project(phelps)

# # Ensure that Emscripten is being used
# if(NOT DEFINED EMSCRIPTEN)
#     message(FATAL_ERROR "This CMakeLists.txt file is designed for Emscripten. Please use emcmake.")
# endif()

# set(CMAKE_CXX_STANDARD 20)

# # Set the path to ImGui
# set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

# # ImGui source files
# set(IMGUI_SOURCES
#     ${IMGUI_PATH}/imgui.cpp
#     ${IMGUI_PATH}/imgui_demo.cpp
#     ${IMGUI_PATH}/imgui_draw.cpp
#     ${IMGUI_PATH}/imgui_tables.cpp
#     ${IMGUI_PATH}/imgui_widgets.cpp
#     ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
#     ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
# )

# # Your project source files
# file(GLOB PROJECT_SOURCES src/*.cpp)

# # Combine all sources
# set(SOURCES ${PROJECT_SOURCES} ${IMGUI_SOURCES})

# add_executable(${PROJECT_NAME} ${SOURCES})

# target_include_directories(${PROJECT_NAME} PRIVATE 
#     include
#     ${IMGUI_PATH}
#     ${IMGUI_PATH}/backends
# )

# # Emscripten-specific compile and link flags
# set(EMSCRIPTEN_FLAGS
#     "-s USE_SDL=2"
#     "-s USE_WEBGL2=1"
#     "-s FULL_ES3=1"
#     "-s WASM=1"
#     "-s ALLOW_MEMORY_GROWTH=1"
#     "-s NO_EXIT_RUNTIME=1"
#     "-s DISABLE_EXCEPTION_CATCHING=1"
#     "--preload-file ${CMAKE_SOURCE_DIR}/assets@/assets"
# )

# # Join the flags into a single string
# string(REPLACE ";" " " EMSCRIPTEN_FLAGS_STRING "${EMSCRIPTEN_FLAGS}")

# # Set compile options
# set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${EMSCRIPTEN_FLAGS_STRING})
# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${EMSCRIPTEN_FLAGS_STRING})

# # Additional link flags for the final executable
# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${EMSCRIPTEN_FLAGS_STRING} -s EXPORTED_RUNTIME_METHODS=['cwrap']")
# # 