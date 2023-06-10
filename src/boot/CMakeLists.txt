project(Header)

add_executable(rom_head.bin
	rom_head.c
)	
target_include_directories(rom_head.bin
	PUBLIC 
		/opt/gendev/sgdk/inc
		/opt/gendev/sgdk/res
)
target_link_options(rom_head.bin
	PUBLIC
		-T /opt/gendev/sgdk/md.ld -nostdlib -Wl,--oformat=binary		
)

add_library(rom_head OBJECT
	sega.s)
target_include_directories(rom_head PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )
add_dependencies(rom_head rom_head.bin)