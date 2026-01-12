#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SFML/Graphics.hpp>

#define HBAR_WIDTH 69.0f
#define HBAR_HEIGHT 5.0f

class HealthBar
{
public:
    int currentHealth;
    int maxHealth;

    sf::RectangleShape healthRect;
    sf::RectangleShape fullRect;

    HealthBar();
    HealthBar(int maxHealth);
    void draw(sf::RenderWindow& window);
    void setHealth(int newHealth);
    void setPosition(sf::Vector2f pos);
};

#endif