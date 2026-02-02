#include "../include/gui/BulletMeter.hpp"

BulletMeter::BulletMeter(sf::Vector2f position, int maxBullets)
{
    this->position = position;
    this->maxBullets = maxBullets;
    this->initSprites(this->maxBullets);
}

void BulletMeter::initSprites(int maxBullets)
{
    this->maxBullets = maxBullets;
    this->currentBullets = maxBullets;
    this->sprites.clear();
    for(int i = 0; i < this->maxBullets; i++)
    {
        this->sprites.push_back(sf::RectangleShape(sf::Vector2f(BULLET_SPRITE_WIDTH, BULLET_SPRITE_HEIGHT)));
        this->sprites[i].setPosition({this->position.x + i * (BULLET_SPRITE_WIDTH + BULLET_SPRITE_DISTANCE), this->position.y});
        this->sprites[i].setFillColor(sf::Color::White);
    }
}

void BulletMeter::draw(sf::RenderWindow& window)
{
    for(int i = 0; i < this->maxBullets; i++)
    {
        window.draw(this->sprites[i]);
    }
}

void BulletMeter::setCurrentBullets(int newCurrentBullets)
{
    for(int i = 0; i < (this->maxBullets - newCurrentBullets); i++)
    {
        this->sprites[i].setFillColor(sf::Color(50, 50, 50));
    }
    this->currentBullets = newCurrentBullets;
}