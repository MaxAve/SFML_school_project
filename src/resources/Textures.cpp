#include "resources/Textures.hpp"
#include <iostream>

namespace Textures {

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

    for (int i = 0; i < textureNames.size(); i++) {
        std::string path = "resources/textures/" + textureNames[i] + ".png";
        std::cout << "[LOG] Loading " << path << "\n";
        stringToTexture.insert({textureNames[i], sf::Texture(path)});
    }
}

sf::Texture* get(std::string texName) {
    if (stringToTexture.find(texName) == stringToTexture.end())
        return nullptr;
    return &stringToTexture.at(texName);
}

} // namespace Textures