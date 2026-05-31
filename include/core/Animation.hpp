#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
private:
    sf::Texture spriteSheet;
    int fps;
    std::vector<int> animationLengths;
    sf::Vector2i spriteSize;

    int currentFrame;
    int currentCycle;
    float timeSinceLastFrame;

public:
    Animation();
    Animation(std::string spriteSheetPath, std::vector<int> animationLengths, int fps, sf::Vector2i spriteSize);
    void setAnimationCycle(int cycle);
    void update(sf::Sprite* targetSprite);
};