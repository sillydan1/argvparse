cmake_minimum_required(VERSION 3.12)
project(argvparse)

set(CMAKE_CXX_STANDARD 17)
add_library(argvparse SHARED src/arguments.cpp)
add_executable(example_usage src/main.cpp)
target_link_libraries(example_usage argvparse)

install(TARGETS argvparse DESTINATION lib)
install(FILES src/arguments.h DESTINATION include)
