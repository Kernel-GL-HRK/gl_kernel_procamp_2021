cmake_minimum_required(VERSION 3.19)
project(rock_paper_scissors)

set(CMAKE_CXX_STANDARD 14)

file(GLOB sources_main "src/*.h" "src/*.cpp")
file(GLOB sources "src/**/*.h" "src/**/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(rock_paper_scissors ${sources_main} ${sources})

