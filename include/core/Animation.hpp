#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
private:
    int fps;
    std::vector<int> animationLengths;
    sf::Vector2i spriteSize;

    int currentFrame;
    int currentCycle;
    float timeSinceLastFrame;

public:
    sf::Texture spriteSheet;

    Animation();
    Animation(std::string spriteSheetPath, std::vector<int> animationLengths, int fps, sf::Vector2i spriteSize);
    void setAnimationCycle(int cycle);
    void update(sf::Sprite* targetSprite, float dt);
    void setDefaultSprite(sf::Sprite* targetSprite);
    inline int getCurrentCycle() { return this->currentCycle; }
};