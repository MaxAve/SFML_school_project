#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>

namespace Textures {

enum TextureType {
    Grass,
    Wood,
    Stone,
    GUI_Q_KEY_PROMPT,
};

extern std::unordered_map<TextureType, sf::Texture> typeToTexture;
extern std::unordered_map<std::string, sf::Texture> stringToTexture;

void initTextures();

// depending on TextureType returns the corresponding texture
sf::Texture* get(TextureType type);
sf::Texture* get(std::string texName);
} // namespace Textures