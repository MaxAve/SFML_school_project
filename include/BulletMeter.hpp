#include <SFML/Graphics.hpp>
#include <vector>

#define BULLET_SPRITE_WIDTH 8
#define BULLET_SPRITE_DISTANCE 4
#define BULLET_SPRITE_HEIGHT 30

class BulletMeter
{
public:
    sf::Vector2f position;
    int maxBullets;
    int currentBullets;
    std::vector<sf::RectangleShape> sprites;

    BulletMeter(sf::Vector2f position, int maxBullets);
    void initSprites(int maxBullets);
    void draw(sf::RenderWindow& window);
    void setCurrentBullets(int newCurrentBullets);
};