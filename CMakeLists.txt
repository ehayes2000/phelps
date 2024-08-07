cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)      
project(example
  LANGUAGES CXX
  VERSION 1.0
)

add_subdirectory(dependencies)
add_subdirectory(src)