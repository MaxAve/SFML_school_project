#pragma once
#include "Textures.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>


struct Tile {
    sf::Sprite sprite;

    Tile();

    Tile(Textures::TextureType);

    void setSize(const sf::Vector2u&);

    void setPosition(const sf::Vector2f&);

    Textures::TextureType getId() const;

    void setTexture(Textures::TextureType);

private:
    Textures::TextureType id;
};