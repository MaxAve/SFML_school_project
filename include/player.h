#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "window.h"

class Player
{
public:
    sf::RectangleShape sprite; // Replace with sprite
    sf::View view;
    sf::Vector2f velocity;
    float speed;

	sf::RectangleShape gunSprite; // Replace with sprite

    Player(sf::RenderWindow& window);

	void update();
    void draw(sf::RenderWindow& window);
};

#endif
