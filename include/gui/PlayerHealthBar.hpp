#pragma once
#include "HealthBar.hpp"
#include "resources/Fonts.hpp"
#include <SFML/Graphics.hpp>

class PlayerHealthBar {
    HealthBar healthBar;
    const unsigned maxHealth;
    sf::Vector2f size;

public:
    PlayerHealthBar(sf::Vector2f _size,unsigned _maxHealth);

    unsigned getHealth() const;

    void setHealth(unsigned newHealth);

    // guarantees that health wont be < 0 or > than maximum
    // accepts a positive or negative value that gets added to hp
    void changeHealth(int changeVal);

    void draw(sf::RenderWindow& window);
};
