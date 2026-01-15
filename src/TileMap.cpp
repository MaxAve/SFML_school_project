#include "../include/TileMap.hpp"
#include <iostream>

TileMap::TileMap(size_t _mapWidth, size_t _mapHeight, float _gridSize)
    : WIDTH{_mapWidth}, HEIGHT{_mapHeight}, gridSizeF{_gridSize}, gridSizeU{static_cast<unsigned>(_gridSize)} {
    tileMap.resize(WIDTH);
    for (size_t x = 0; x < WIDTH; x++) {
        tileMap[x].resize(HEIGHT);
        for (size_t y = 0; y < HEIGHT; y++) {
            tileMap[x][y].setTexture(Textures::TextureType::Stone);
            tileMap[x][y].setSize({(unsigned)gridSizeF, (unsigned)gridSizeF});
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

void TileMap::draw(sf::RenderWindow& window, long long int fromX, long long int toX, long long int fromY, long long int toY) {
    if (fromX <= 0)
        fromX = 0;

    if (fromY <= 0)
        fromY = 0;

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

void TileMap::draw(sf::RenderWindow& window) {
    const sf::View& view = window.getView();
    long long int fromX = (view.getCenter().x - window.getSize().x / 2) / gridSizeU;
    long long int toX = (view.getCenter().x + window.getSize().x / 2) / gridSizeU + 1;
    long long int fromY = (view.getCenter().y - window.getSize().y / 2) / gridSizeU;
    long long int toY = (view.getCenter().y + window.getSize().y / 2) / gridSizeU + 1;

    draw(window, fromX, toX, fromY, toY);
}