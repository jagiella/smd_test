
add_megadrive_rom(tetris
	tetris.cpp)
	
target_include_directories(dungeon 
	PUBLIC
		${PROJECT_SOURCE_DIR}/src/lib
)
target_link_libraries(dungeon
	lib)