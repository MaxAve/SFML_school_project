#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace Textures {

enum TextureType {
    Grass,
    Wood,
    Stone,
    GUI_Q_KEY_PROMPT,
};

extern std::unordered_map<TextureType, sf::Texture> typeToTexture;

void initTextures();

// depending on TextureType returns the corresponding texture
sf::Texture* get(TextureType type);
} // namespace Textures