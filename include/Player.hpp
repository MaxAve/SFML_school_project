#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Window.hpp"
#include "gui/Inventory.hpp"
#include "Hitbox.hpp"
#include <iostream>

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
    Inventory inventory;
    bool reloading;
    Hitbox hitbox;

	sf::RectangleShape gunSprite; // Replace with sprite

    Player(sf::RenderWindow& window);

	void update();
    void draw(sf::RenderWindow& window);
    void setPosition(sf::Vector2f pos);
    void move(sf::Vector2f delta);
};