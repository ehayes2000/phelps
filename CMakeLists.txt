# cmake_minimum_required(VERSION 3.10)
# project(phelps)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# add_subdirectory(SDL)
# # Find SDL2 package
# find_package(SDL2 REQUIRED)
# include_directories(SDL/include)

# # Find SDL2_image, SDL2_ttf, and SDL2_mixer if you need them
# find_package(SDL2_image REQUIRED)
# find_package(SDL2_ttf REQUIRED)
# find_package(SDL2_mixer REQUIRED)

# file(GLOB_RECURSE SOURCES "src/*.cpp")

# add_executable(${PROJECT_NAME} ${SOURCES})

# target_include_directories(${PROJECT_NAME} PRIVATE src)

# # Link SDL2 and additional libraries
# target_link_libraries(${PROJECT_NAME}  ${CMAKE_CURRENT_SOURCE_DIR}/SDL/include)
cmake_minimum_required(VERSION 3.20)
project(phelps VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.26.5  # You can change this to a specific version or branch
)

FetchContent_MakeAvailable(SDL2)

add_executable(phelps src/main.cpp)

target_link_libraries(phelps PRIVATE SDL2::SDL2main SDL2::SDL2)