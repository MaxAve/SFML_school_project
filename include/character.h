#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "animation.h"

class Character
{
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Sprite sprite;
    Animation animation;
};