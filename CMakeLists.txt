cmake_minimum_required(VERSION 3.13)
project(chip8)

set(CMAKE_CXX_STANDARD 17)

AUX_SOURCE_DIRECTORY(src EXECUTABLES)
add_executable(chip8 main.cpp ${EXECUTABLES})
find_package(SDL2 REQUIRED)
target_link_libraries(chip8 SDL2)
