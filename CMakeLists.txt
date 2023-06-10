cmake_minimum_required(VERSION 3.25)
project(smd_test
	C CXX ASM)

enable_language(C ASM)

add_subdirectory(src/boot)

add_executable(rom.out
	fixpoint.c
	smd_test.cpp
	)
target_include_directories(rom.out
	PUBLIC
		/opt/gendev/sgdk/inc
		/opt/gendev/sgdk/res)
target_compile_options(rom.out
	PUBLIC
	-O5 -fuse-linker-plugin -fno-web -fno-gcse -fno-unit-at-a-time -fomit-frame-pointer -MMD
	)
target_link_libraries(rom.out
	rom_head md gcc)	
target_link_directories(rom.out
	PUBLIC
		/opt/gendev/sgdk/lib/)
target_link_options(rom.out
	PUBLIC
		 -T /opt/gendev/sgdk/md.ld -nostdlib -Wl,--gc-sections,--build-id=none)

add_custom_command(
    TARGET rom.out
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary rom.out ${PROJECT_NAME}.bin
)