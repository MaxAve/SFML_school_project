cmake_minimum_required(VERSION 3.22)
project(sfml_test LANGUAGES CXX)

set(SFML_STATIC_LIBRARIES ON)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/lib/cmake/SFML")
find_package(SFML 3 CONFIG REQUIRED COMPONENTS System Window Graphics)

add_executable(test src/main.cpp 
					src/Bullet.cpp 
					src/Physics.cpp 
					src/Player.cpp 
					src/Tile.cpp 
					src/TileMap.cpp 
					src/window.cpp
					src/Zombie.cpp
					src/utils.cpp)
target_link_libraries(test PRIVATE SFML::System SFML::Window SFML::Graphics)

