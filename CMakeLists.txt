cmake_minimum_required(VERSION 3.22)
project(sfml_test LANGUAGES CXX)

set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics)

add_executable(test src/main.cpp 
					src/Bullet.cpp 
					src/Physics.cpp 
					src/Player.cpp 
					src/Tile.cpp 
					src/TileMap.cpp 
					src/window.cpp)
target_link_libraries(test PRIVATE SFML::System SFML::Window SFML::Graphics)

