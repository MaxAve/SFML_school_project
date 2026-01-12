#pragma once
#include "Textures.hpp"
#include <SFML/Graphics.hpp>

struct Tile {
    sf::Sprite sprite;

    Tile();

    void setSize(const sf::Vector2f&);

    void setPosition(const sf::Vector2f&);

private:
    size_t id;
    static size_t tileCnt;
};