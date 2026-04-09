cmake_minimum_required(VERSION 3.22)
project(game LANGUAGES CXX)

set(SFML_STATIC_LIBRARIES ON)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/lib/cmake/SFML")
find_package(SFML 3 CONFIG REQUIRED COMPONENTS System Window Graphics)

add_executable(game src/main.cpp 
					src/entities/Bullet.cpp 
					src/core/Physics.cpp 
					src/entities/Player.cpp 
					src/environment/Tile.cpp 
					src/core/Window.cpp
					src/resources/Textures.cpp
					src/entities/Zombie.cpp
					src/core/Utils.cpp
					src/gui/HealthBar.cpp
					src/resources/Fonts.cpp
					src/gui/PlayerHealthBar.cpp
					src/fx/Particle.cpp
					src/looting/Item.cpp
					src/looting/Inventory.cpp
					src/gui/InventoryInterface.cpp
					src/gui/SharedInventoryInterface.cpp
					src/looting/InventorySlot.cpp
					src/gui/InventorySlotGui.cpp
					src/core/Hitbox.cpp
					src/gui/BulletMeter.cpp
					src/environment/Door.cpp
					src/looting/LootContainer.cpp
					src/fx/DamageIndicatorText.cpp
					src/environment/TileMapChunk.cpp)
target_link_libraries(game PRIVATE SFML::System SFML::Window SFML::Graphics)