cmake_minimum_required(VERSION 3.29)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src main.cpp
        iolib/std.cpp
        iolib/std.h)
