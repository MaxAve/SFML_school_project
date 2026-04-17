#pragma once

#include <SFML/Graphics.hpp>
#include "resources/Fonts.hpp"
#include "core/Window.hpp"
#include <string>
#include <iostream>

class ItemLabel
{
public:
    static bool visible;
    static sf::Text itemName;
    static sf::Text itemDescription;
    static sf::RectangleShape box;
    static sf::RectangleShape box2;

    static void init();
    static void update(std::string name, std::string desc);
    static void draw(sf::RenderWindow& window);
};