#include "../include/TileMap.hpp"
#include <iostream>



TileMap::TileMap(size_t _mapWidth, size_t _mapHeight, float _gridSize)
    : WIDTH{_mapWidth}, HEIGHT{_mapHeight}, gridSizeF{_gridSize} {
    tileMap.resize(WIDTH);
    for (size_t x = 0; x < WIDTH; x++) {
        tileMap[x].resize(HEIGHT);
        for (size_t y = 0; y < HEIGHT; y++) {
            tileMap[x][y].setTexture(Textures::TextureType::Wood);
            // tileMap[x][y].setSize({gridSizeF, gridSizeF});
            tileMap[x][y].setPosition({x * gridSizeF, y * gridSizeF});
        }
    }
}

const Tile* TileMap::getTile(size_t x, size_t y) {
    if (x >= WIDTH || y >= HEIGHT) {
        return nullptr;
    }

    return &(tileMap[x][y]);
}

void TileMap::drawAll(sf::RenderWindow& window, size_t fromX, size_t toX, size_t fromY, size_t toY) {
    if (fromX >= WIDTH)
        fromX = WIDTH;

    if (fromY >= HEIGHT)
        fromY = HEIGHT;

    if (toX >= WIDTH)
        toX = WIDTH;

    if (toY >= HEIGHT)
        toY = HEIGHT;

    for (size_t x = fromX; x < toX; x++) {
        for (size_t y = fromY; y < toY; y++) {
            window.draw(tileMap[x][y].sprite);
        }
    }
}

void TileMap::drawAll(sf::RenderWindow& window) {
    drawAll(window, 0, WIDTH, 0, HEIGHT);
}