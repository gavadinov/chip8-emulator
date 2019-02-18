cmake_minimum_required(VERSION 3.12)
project(chip8)
set(CMAKE_CXX_STANDARD 17)

AUX_SOURCE_DIRECTORY(src EXECUTABLES)
add_executable(chip8 main.cpp ${EXECUTABLES})
find_package(SDL2 CONFIG REQUIRED)

# Create imported target SDL2::SDL2_ttf
add_library(SDL2::SDL2_ttf SHARED IMPORTED)
set_target_properties(SDL2::SDL2_ttf PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_INCLUDE_PATH}/SDL2"
  IMPORTED_IMPLIB "${CURRENT_CONFIGURATION_PATH}/lib/SDL2_ttf.lib"
  IMPORTED_LOCATION "${CURRENT_CONFIGURATION_PATH}/bin/SDL2_ttf.dll"
)

target_link_libraries(chip8 PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2_ttf)
