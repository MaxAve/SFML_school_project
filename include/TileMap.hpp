#pragma once
#include "Tile.hpp"
#include "Textures.hpp"
#include <SFML/Graphics.hpp>

class TileMap {
    // in amount of tiles
    const size_t WIDTH;
    const size_t HEIGHT;

    const float gridSizeF;
    const unsigned gridSizeU;

    std::vector<std::vector<Tile>> tileMap;

public:
    TileMap(size_t _mapWidth, size_t _mapHeight, float _gridSize);

    const Tile* getTile(size_t x, size_t y);

    void draw(sf::RenderWindow& window);

    void draw(sf::RenderWindow& window, long long int fromX, long long int toX, long long int fromY, long long int toY);
};