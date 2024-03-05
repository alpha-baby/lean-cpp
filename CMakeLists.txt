cmake_minimum_required(VERSION 3.26)
project(lean_cpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(lean_cpp main.cpp
        main.h
        main.c)
