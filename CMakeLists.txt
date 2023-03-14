cmake_minimum_required(VERSION 3.20)
project(ROCK_SLIDE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}>/bin)
add_executable(main src/main.cpp src/glad.c)
target_include_directories(main PRIVATE include)
target_link_directories(main PRIVATE lib)
target_link_libraries(main LINK_PUBLIC glfw3 opengl32)

