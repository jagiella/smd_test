# the name of the target operating system
set(CMAKE_SYSTEM_NAME Linux)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER   m68k-linux-gnu-gcc-13)
set(CMAKE_CXX_COMPILER m68k-linux-gnu-g++-13)
set(CMAKE_ASM_COMPILER m68k-linux-gnu-gcc-13)
set(CMAKE_LINKER m68k-linux-gnu-gcc-13)
#set(CMAKE_CXX_CLANG_TIDY clang-tidy;
#	-warnings-as-errors=*;
#	-checks=-*,readability*,cppcoreguidelines*,-*-magic-numbers,-clang-diagnostic-ignored-optimization-argument,-llvmlibc*)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  /usr/bin)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)



set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

SET(CMAKE_C_FLAGS "-m68000 -Wall -Wextra -Wno-shift-negative-value -Wno-main -Wno-unused-parameter -fno-builtin -Wpedantic -fno-exceptions")
SET(CMAKE_CXX_FLAGS "-m68000 -Wall -Wextra -Wno-shift-negative-value -Wno-main -Wno-unused-parameter -fno-builtin -Wpedantic -fno-exceptions")
SET(ASM_OPTIONS "-x assembler-with-cpp")
SET(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} ${ASM_OPTIONS}")


# rom header
project(Header)
add_executable(rom_head.bin
	/opt/gendev/sgdk/src/boot/rom_head.c
)	
target_include_directories(rom_head.bin SYSTEM
	PUBLIC 
		/opt/gendev/sgdk/inc
		/opt/gendev/sgdk/res
)
target_link_options(rom_head.bin
	PUBLIC
		-T /opt/gendev/sgdk/md.ld -nostdlib -Wl,--oformat=binary		
)
set_target_properties( rom_head.bin
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out"
)

add_library(rom_head OBJECT
	/opt/gendev/sgdk/src/boot/sega.s)
target_include_directories(rom_head PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )
add_dependencies(rom_head rom_head.bin)

# Build a Sega Megadrive/Genesis rom:
# add_rom(<name> source1 [source2 ...])
function(add_rom target_name)

	set(rom_name "${target_name}.out")
	add_executable(${rom_name}
		${ARGN}
		)
	target_include_directories(${rom_name}
		PUBLIC
			/opt/gendev/sgdk/inc
			/opt/gendev/sgdk/res)
	target_compile_options(${rom_name}
		PUBLIC
		-O5 -fuse-linker-plugin -fno-web -fno-gcse -fno-unit-at-a-time -fomit-frame-pointer -MMD
		)
	target_link_libraries(${rom_name}
		rom_head md gcc)	
	target_link_directories(${rom_name}
		PUBLIC
			/opt/gendev/sgdk/lib/)
	target_link_options(${rom_name}
		PUBLIC
			 -n -T /opt/gendev/sgdk/md.ld -nostdlib -Wl,--gc-sections,--build-id=none,-znoexecstack)
	
	add_custom_command(
	    TARGET ${rom_name}
	    BYPRODUCTS ${target_name}.bin
	    POST_BUILD
	    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${rom_name} ${target_name}.bin
	)
endfunction()


