
add_megadrive_rom(tetris
	tetris.cpp)
	
target_include_directories(tetris 
	PUBLIC
		${PROJECT_SOURCE_DIR}/src/lib
)
target_link_libraries(tetris
	lib)