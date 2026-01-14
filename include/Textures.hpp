#pragma once
#include <SFML/Graphics.hpp>

namespace Textures {

enum TextureType {
    Grass,
    Wood,
    Stone,
};

extern sf::Texture grass;
extern sf::Texture wood;
extern sf::Texture stone;

void initTextures();

// depending on TextureType returns the corresponding texture
sf::Texture* get(TextureType type);
} // namespace Textures