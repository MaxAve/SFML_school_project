#include "../include/Textures.hpp"
#include <iostream>

namespace Textures {
sf::Texture grass;

void initTextures() {
    if (!grass.loadFromFile("resources/textures/grass.png")) {
        std::cerr << "Error: Texture \"grass.png\"" << std::endl;
    }
    grass.setSmooth(true);
}
} // namespace Textures