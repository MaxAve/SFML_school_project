#include "resources/Textures.hpp"
#include <iostream>

namespace Textures {

std::unordered_map<TextureType, sf::Texture> typeToTexture;

std::unordered_map<std::string, sf::Texture> stringToTexture;

void initTextures() {
    std::vector<std::string> textureNames = {
        "items/bandage",
        "items/bullet_large",
        "items/bullet_medium",
        "items/bullet_small",
        "items/bullet_spread",
        "items/medkit",
        "items/medkit3d",
        "items/kitchen_knife",
        "items/combat_knife",
        "items/lockpick",
        "items/scope",
        "items/gun_smg",
        "items/gun_ar",
        "items/gun_revolver",
        "gui_hud/q_key_prompt",
    };
    for(int i = 0; i < textureNames.size(); i++)
    {
        std::string path = "resources/textures/" + textureNames[i] + ".png";
        std::cout << "Loading " << path << "\n";
        stringToTexture.insert({textureNames[i], sf::Texture(path)});
    }

    // TODO remove all of this:
    // if (!typeToTexture[Textures::Grass].loadFromFile("resources/textures/grass.png")) {
    //     std::cerr << "Error: Texture \"grass.jpg\"" << std::endl;
    // }
    // typeToTexture[Textures::Grass].setSmooth(false);

    // if (!typeToTexture[Textures::Wood].loadFromFile("resources/textures/wood.jpg")) {
    //     std::cerr << "Error: Texture \"wood.jpg\"" << std::endl;
    // }
    // typeToTexture[Textures::Wood].setSmooth(false);

    // if (!typeToTexture[Textures::Stone].loadFromFile("resources/textures/stone.jpg")) {
    //     std::cerr << "Error: Texture \"stone.jpg\"" << std::endl;
    // }
    // typeToTexture[Textures::Stone].setSmooth(false);

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

sf::Texture* get(std::string texName)
{
    if (stringToTexture.find(texName) == stringToTexture.end())
        return nullptr;
    return &stringToTexture.at(texName);
}

} // namespace Textures