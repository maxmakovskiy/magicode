cmake_minimum_required(VERSION 3.2)
project(part1 C)
set(CMAKE_BUILD_TYPE Debug)
add_executable(main main.c stack.c file.c)
