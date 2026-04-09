#include "resources/Textures.hpp"
#include <iostream>

namespace Textures {

std::unordered_map<TextureType, sf::Texture> typeToTexture;


void initTextures() {
    if (!typeToTexture[Textures::Grass].loadFromFile("resources/textures/grass.jpg")) {
        std::cerr << "Error: Texture \"grass.jpg\"" << std::endl;
    }
    typeToTexture[Textures::Grass].setSmooth(false);

    if (!typeToTexture[Textures::Wood].loadFromFile("resources/textures/wood.jpg")) {
        std::cerr << "Error: Texture \"wood.jpg\"" << std::endl;
    }
    typeToTexture[Textures::Wood].setSmooth(false);

    if (!typeToTexture[Textures::Stone].loadFromFile("resources/textures/stone.jpg")) {
        std::cerr << "Error: Texture \"stone.jpg\"" << std::endl;
    }
    typeToTexture[Textures::Stone].setSmooth(false);

    if (!typeToTexture[Textures::GUI_Q_KEY_PROMPT].loadFromFile("resources/textures/gui_hud/q_key_prompt.png")) {
        std::cerr << "Error: Texture \"gui_hud/q_key_prompt.jpg\"" << std::endl;
    }
    typeToTexture[Textures::GUI_Q_KEY_PROMPT].setSmooth(false);
}

sf::Texture* get(TextureType type) {
    if (typeToTexture.find(type) == typeToTexture.end()) {
        return nullptr;
    }
    
    return &typeToTexture[type];
}
} // namespace Textures