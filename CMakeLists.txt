cmake_minimum_required(VERSION 3.15)
project(argvparse)

set(CMAKE_CXX_STANDARD 20)
add_library(libargvparse src/arguments.cpp)
add_executable(example_usage src/main.cpp)
target_link_libraries(example_usage libargvparse)
