#pragma once

#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720

extern sf::RenderWindow window;
extern sf::View defaultView;

class Window
{
public:
    static sf::Vector2i getMousePos(const sf::View& view = defaultView);
    static sf::Vector2i getMousePosFromCenter(const sf::View& view = defaultView);
};