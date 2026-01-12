#include "../include/Textures.hpp"
#include <iostream>

namespace Textures {
sf::Texture grass;
sf::Texture wood;
sf::Texture stone;

void initTextures() {
    if (!grass.loadFromFile("resources/textures/grass.png")) {
        std::cerr << "Error: Texture \"grass.png\"" << std::endl;
    }
    grass.setSmooth(true);

    if (!wood.loadFromFile("resources/textures/wood.jpg")) {
        std::cerr << "Error: Texture \"wood.jpg\"" << std::endl;
    }
    wood.setSmooth(true);

    if (!stone.loadFromFile("resources/textures/stone.jpg")) {
        std::cerr << "Error: Texture \"stone.jpg\"" << std::endl;
    }
    stone.setSmooth(true);
}

sf::Texture* get(TextureType type) {
    switch (type) {
        case TextureType::Grass:
        return &grass;
        case TextureType::Wood:
        return &wood;
        case TextureType::Stone:
        return &stone;
        default:
        std::cout << "Textures::get(TextureType): No such type" << std::endl;
    }

    return nullptr;
}
} // namespace Textures