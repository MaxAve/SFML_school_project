#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>

namespace Textures {

extern std::unordered_map<std::string, sf::Texture> stringToTexture;

void initTextures();

// depending on TextureType returns the corresponding texture
sf::Texture* get(std::string texName);
} // namespace Textures