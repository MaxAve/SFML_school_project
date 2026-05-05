#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "resources/Fonts.hpp"
#include "core/Physics.hpp"

#define BULLET_SPRITE_WIDTH 10.0f
#define BULLET_SPRITE_DISTANCE 0.0f
#define BULLET_SPRITE_HEIGHT 40.0f
#define BAR_WIDTH 498.0f

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