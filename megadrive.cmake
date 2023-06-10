# the name of the target operating system
set(CMAKE_SYSTEM_NAME Megadrive)

#enable_language(C ASM)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  /usr/bin)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# which compilers to use for C and C++
set(CMAKE_C_COMPILER   m68k-linux-gnu-gcc-13)
set(CMAKE_CXX_COMPILER m68k-linux-gnu-g++-13)
set(CMAKE_ASM_COMPILER m68k-linux-gnu-gcc-13)
set(CMAKE_LINKER m68k-linux-gnu-gcc-13)
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

SET(CMAKE_C_FLAGS "-m68000 -Wall -Wextra -Wno-shift-negative-value -Wno-main -Wno-unused-parameter -fno-builtin")
SET(CMAKE_CXX_FLAGS "-m68000 -Wall -Wextra -Wno-shift-negative-value -Wno-main -Wno-unused-parameter -fno-builtin")
SET(ASM_OPTIONS "-x assembler-with-cpp")
SET(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} ${ASM_OPTIONS}")

