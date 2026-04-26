cmake_minimum_required(VERSION 3.22)
project(game LANGUAGES CXX)

set(SFML_STATIC_LIBRARIES ON)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/lib/cmake/SFML")
find_package(SFML 3 CONFIG REQUIRED COMPONENTS System Window Graphics)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(game ${SOURCES}
    include/gui/GuiParameters.hpp
    src/gui/GuiParameters.cpp)

target_include_directories(game PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(game PRIVATE SFML::System SFML::Window SFML::Graphics)