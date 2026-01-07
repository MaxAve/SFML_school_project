#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.hpp"

class TileMap {
    // in amount of tiles
    const size_t WIDTH;
    const size_t HEIGHT;

    const float gridSizeF;

    std::vector<std::vector<Tile>> tileMap;

public:
    TileMap(size_t _mapWidth, size_t _mapHeight, float _gridSize);

    const Tile* getTile(size_t x, size_t y);

    void drawAll(sf::RenderWindow& window);

    void drawAll(sf::RenderWindow& window, size_t fromX, size_t toX, size_t fromY, size_t toY);

};