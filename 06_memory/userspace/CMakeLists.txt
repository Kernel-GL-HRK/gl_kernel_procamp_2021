cmake_minimum_required(VERSION 3.21)
project(memory-allocator-test-bench)

set(CMAKE_CXX_STANDARD 17)

add_executable(memory-allocator-test-bench main.cpp period_counter.cpp period_counter.h)
