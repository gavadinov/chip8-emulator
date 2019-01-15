cmake_minimum_required(VERSION 3.13)
project(chip8)

set(CMAKE_CXX_STANDARD 17)

add_executable(chip8 main.cpp src/Chip8.cpp src/Chip8.h)