cmake_minimum_required(VERSION 3.20)
project(phelps VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories to be within the build directory
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

include(FetchContent)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x  # You can change this to a specific version or branch
)

FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(phelps ${SOURCES})

target_link_libraries(phelps PRIVATE sfml-graphics sfml-window sfml-system)

# Ensure SFML DLLs are copied to the output directory on Windows
if(WIN32)
  add_custom_command(TARGET phelps POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:sfml-graphics>
      $<TARGET_FILE:sfml-window>
      $<TARGET_FILE:sfml-system>
      $<TARGET_FILE_DIR:phelps>
  )
endif()