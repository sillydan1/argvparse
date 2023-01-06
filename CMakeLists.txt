# Copyright (C) 2023 Asger Gitz-Johansen
# This file is part of libargvparse.
#
#  libargvparse is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  libargvparse is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with libargvparse.  If not, see <https://www.gnu.org/licenses/>.
cmake_minimum_required(VERSION 3.12)
project(argvparse VERSION 1.2.2)
include(cmake/CPM.cmake)

CPMAddPackage("gh:catchorg/textflowcpp#master")
include_directories(
        ${textflowcpp_SOURCE_DIR}
        src
)

set(CMAKE_CXX_STANDARD 17)
add_library(argvparse STATIC src/arguments.cpp)
add_executable(example_usage src/main.cpp)
target_link_libraries(example_usage argvparse)

install(TARGETS argvparse DESTINATION lib)
install(FILES src/arguments.h DESTINATION include)

