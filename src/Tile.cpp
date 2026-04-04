#include "../include/Tile.hpp"

Tile::Tile() : sprite(*Textures::get(Textures::TextureType::Grass)) {
    id = Textures::TextureType::Grass;
}

Tile::Tile(Textures::TextureType textureType) : sprite(*Textures::get(textureType)) {
    id = textureType;
}

// accepts a vector2 of targeted size, not factor
void Tile::setSize(const sf::Vector2u& size) {
    sprite.setScale({size.x / sprite.getLocalBounds().size.x, size.y / sprite.getLocalBounds().size.y});
}

void Tile::setPosition(const sf::Vector2f& coords) {
    sprite.setPosition(coords);
}

Textures::TextureType Tile::getId() const { return id; }

void Tile::setTexture(Textures::TextureType textureType) {
    sprite.setTexture(*Textures::get(textureType));
    id = textureType;
}