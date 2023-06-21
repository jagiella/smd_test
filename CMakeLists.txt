cmake_minimum_required(VERSION 3.25)

include(megadrive.cmake)

project(smd_test
	C CXX ASM)

add_rom(smd_test
	src/Tiles.cpp
	src/Sprites.cpp
	smd_test.cpp)