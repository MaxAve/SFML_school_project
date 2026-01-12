cmake_minimum_required(VERSION 3.22)
project(game LANGUAGES CXX)

set(SFML_STATIC_LIBRARIES ON)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/lib/cmake/SFML")
find_package(SFML 3 CONFIG REQUIRED COMPONENTS System Window Graphics)

add_executable(game src/main.cpp 
					src/Bullet.cpp 
					src/Physics.cpp 
					src/Player.cpp 
					src/Tile.cpp 
					src/TileMap.cpp 
					src/window.cpp
					src/Textures.cpp
					src/Zombie.cpp
					src/utils.cpp)
target_link_libraries(game PRIVATE SFML::System SFML::Window SFML::Graphics)