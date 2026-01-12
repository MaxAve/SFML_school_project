#include "../include/Tile.hpp"

Tile::Tile()
    : sprite(Textures::grass) {
}

void Tile::setSize(const sf::Vector2f& size) {
    sprite.setScale(size);
}

void Tile::setPosition(const sf::Vector2f& coords) {
    sprite.setPosition(coords);
}
