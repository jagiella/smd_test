add_megadrive_rom(dungeon
	Projectile.cpp
	dungeon.cpp)
target_include_directories(dungeon 
	PUBLIC
		${PROJECT_SOURCE_DIR}/src/lib
)
target_link_libraries(dungeon
	lib)
	
