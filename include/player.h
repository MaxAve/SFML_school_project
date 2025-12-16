#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
public:
    sf::RectangleShape sprite;
    sf::View view;
    sf::Vector2f velocity;
    float speed;

    Player(sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);
};

#endif