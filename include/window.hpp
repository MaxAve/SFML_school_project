#pragma once

#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

extern sf::RenderWindow window;

class Window
{
public:
    static sf::Vector2i getMousePos();
    static sf::Vector2i getMousePosFromCenter();
};