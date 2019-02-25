cmake_minimum_required(VERSION 3.12)
project(chip8)
set(CMAKE_CXX_STANDARD 17)

AUX_SOURCE_DIRECTORY(src EXECUTABLES)
add_executable(chip8 main.cpp ${EXECUTABLES})
find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(chip8 PRIVATE SDL2::SDL2 SDL2::SDL2main)
