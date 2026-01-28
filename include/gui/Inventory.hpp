#pragma once
#include "../Window.hpp"
#include <SFML/Graphics.hpp>

class Inventory {
    static const sf::Color stdBackgroundColor;
    static const sf::Color stdForegroundColor;
    static const float stdOutlineThickness;
    static const sf::Color stdOutlineColor;

    sf::RectangleShape background;
    sf::RectangleShape foreground;

public:
    Inventory(const sf::Vector2f& size, const sf::Vector2f& position = {0, 0});

    void resizeForeground(sf::Vector2f);

    void resizeBackground(sf::Vector2f);

    void setPosition(sf::Vector2f);

    void draw();
};