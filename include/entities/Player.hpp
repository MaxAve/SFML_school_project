#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "core/Window.hpp"
#include "looting/Inventory.hpp"
#include "core/Hitbox.hpp"
#include "looting/Hotbar.hpp"
#include "environment/GameMap.hpp"
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
    Hotbar hotbar;
    bool reloading;
    Hitbox hitbox; // Used for collisions with enemies
    Hitbox envHitbox; // Used for collisions with the map. This hitbox is supposed to be smaller than the other one
    Item* equippedItem;

    // walking sound stuff
    float stepCountdown = 0.f;

	sf::RectangleShape gunSprite; // Replace with sprite

    Player(sf::RenderWindow& window);

	void update();
    void draw(sf::RenderWindow& window);
    void setPosition(sf::Vector2f pos);
    void move(sf::Vector2f delta, const GameMap* map);
};