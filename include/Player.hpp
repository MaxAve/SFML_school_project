#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Window.hpp"

#ifndef M_PI
#define M_PI 3.1415926535
#endif

class Player
{
public:
    sf::RectangleShape sprite;
    sf::Vector2f velocity;
    sf::View view;
    float speed;

	sf::RectangleShape gunSprite; // Replace with sprite

    Player(sf::RenderWindow& window);

	void update();
    void draw(sf::RenderWindow& window);
};