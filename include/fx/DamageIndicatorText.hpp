#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "resources/Fonts.hpp"
#include "core/Physics.hpp"

class DamageIndicatorText
{
public:
    static std::vector<DamageIndicatorText*> pool;
    sf::Text* text;
    float timeRemaining;
    float totalTime;
    bool crit;

    DamageIndicatorText(sf::Vector2f pos, int damage, bool crit);
    static void drawAll(sf::RenderWindow& window);
};