cmake_minimum_required(VERSION 3.12)
project(argvparse)

set(CMAKE_CXX_STANDARD 17)
add_library(libargvparse SHARED src/arguments.cpp)
add_executable(example_usage src/main.cpp)
target_link_libraries(example_usage libargvparse)

install(TARGETS libargvparse DESTINATION lib)
install(FILES src/arguments.h DESTINATION include)
