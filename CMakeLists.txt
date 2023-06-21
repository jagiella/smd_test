cmake_minimum_required(VERSION 3.25)

include(megadrive.cmake)

project(smd_test
	C CXX ASM)

add_rom(smd_test
	smd_test.cpp)