#include <SFML/Graphics.hpp>
#include <vector>
#include "fonts.hpp"
#include "Physics.hpp"

#define BULLET_SPRITE_WIDTH 10
#define BULLET_SPRITE_DISTANCE 6
#define BULLET_SPRITE_HEIGHT 40

class BulletMeter
{
public:
    sf::Vector2f position;
    int maxBullets;
    int currentBullets;
    std::vector<sf::RectangleShape> sprites;
    std::vector<sf::RectangleShape> ejectedBulletSprites; // animation

    BulletMeter(sf::Vector2f position, int maxBullets);
    void initSprites(int maxBullets);
    void draw(sf::RenderWindow& window);
    void setCurrentBullets(int newCurrentBullets);
    void updateAnimations();
    void ejectBullet(int bulletPos);
};