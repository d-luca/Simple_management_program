# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10.2)

# Project's name, version, and lenguages
project(Davi'_Luca_Gestionale VERSION 1.0 LANGUAGES C)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Enable C99 standard
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

add_library (Menu menu.c)
add_library (File file.c)
add_library (Strutture strutture.c)
add_library (Ordinamento ordinamento.c)

add_executable(Gestionale main.c)

target_link_libraries (Gestionale Menu)
target_link_libraries (Gestionale File)
target_link_libraries (Gestionale Strutture)
target_link_libraries (Gestionale Ordinamento)
