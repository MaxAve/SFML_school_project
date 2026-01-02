#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
extern sf::RenderWindow window;

sf::Vector2i getMousePos();
sf::Vector2i getMousePosFromCenter();

#endif
