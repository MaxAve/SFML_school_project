#include "../include/Tile.hpp"

Tile::Tile() : sprite(Textures::grass) {
    id = Textures::TextureType::Grass;
}

Tile::Tile(Textures::TextureType textureType) : sprite(*Textures::get(textureType)) {
    id = textureType;
}

void Tile::setSize(const sf::Vector2f& size) {
    sprite.scale(size);
}

void Tile::setPosition(const sf::Vector2f& coords) {
    sprite.setPosition(coords);
}

Textures::TextureType Tile::getId() const { return id; }

void Tile::setTexture(Textures::TextureType textureType) {
    sprite.setTexture(*Textures::get(textureType));
    id = textureType;
}