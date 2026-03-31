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
					src/Window.cpp
					src/Textures.cpp
					src/Zombie.cpp
					src/Utils.cpp
					src/HealthBar.cpp
					src/fonts.cpp
					src/gui/PlayerHealthBar.cpp
					src/fx/Particle.cpp
					src/Item.cpp
					src/Inventory.cpp
					src/gui/InventoryInterface.cpp
					src/InventorySlot.cpp
					src/gui/InventorySlotGui.cpp
					src/Hitbox.cpp
					src/gui/BulletMeter.cpp
					src/Door.cpp
					src/LootContainer.cpp
					src/fx/DamageIndicatorText.cpp)
target_link_libraries(game PRIVATE SFML::System SFML::Window SFML::Graphics)
