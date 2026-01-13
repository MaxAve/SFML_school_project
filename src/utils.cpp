#include "../include/utils.hpp"

float utils::distance(sf::Vector2f a, sf::Vector2f b)
{
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}