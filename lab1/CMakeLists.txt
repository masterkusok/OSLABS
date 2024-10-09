cmake_minimum_required(VERSION 3.29)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab1 main.cpp
        iolib/std.cpp
        iolib/std.h
        errlib/errors.h
        errlib/error_handlers.cpp
        errlib/error_handlers.h)
