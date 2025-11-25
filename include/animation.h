#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>

class Animation
{
public:
    sf::Texture* currentTexture;
    std::vector<sf::Texture> cycle;
};