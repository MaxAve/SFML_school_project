#pragma once

#include <SFML/Graphics.hpp>
#include "core/Physics.hpp"
#include <iostream>

#define HBAR_WIDTH 69.0f
#define HBAR_HEIGHT 5.0f

class HealthBar
{
public:
    int currentHealth;
    int maxHealth;

    sf::RectangleShape healthRect; // Green bar
    sf::RectangleShape fullRect; // Red background

    // Blinking animation variables (activated upon recieving damage)
    sf::RectangleShape healthChangeAnimation;
    float timeSinceHealthChangeAnimationBlink;
    int blinkTimes;
    float healthChangeAnimationDeltaX;

    HealthBar();
    HealthBar(int maxHealth);
    void draw(sf::RenderWindow& window);
    void setHealth(int newHealth);
    void setPosition(sf::Vector2f pos);
};