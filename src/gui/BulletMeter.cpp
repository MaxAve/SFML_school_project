#include "../../include/gui/BulletMeter.hpp"

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
        this->sprites[i].setOutlineThickness(1);
        this->sprites[i].setOutlineColor(sf::Color::White);
    }
}

void BulletMeter::draw(sf::RenderWindow& window)
{
    for(int i = 0; i < this->maxBullets; i++)
    {
        window.draw(this->sprites[i]);
    }

    for(int i = 0; i < this->ejectedBulletSprites.size(); i++)
    {
        window.draw(this->ejectedBulletSprites[i]);
    }
}

void BulletMeter::setCurrentBullets(int newCurrentBullets)
{
    for(int i = 0; i < (this->maxBullets - newCurrentBullets); i++)
    {
        this->sprites[i].setFillColor(sf::Color::Transparent);
    }
    this->currentBullets = newCurrentBullets;
}

void BulletMeter::updateAnimations()
{
    for(int i = 0; i < this->ejectedBulletSprites.size(); i++)
    {
        this->ejectedBulletSprites[i].move({0, 400 * Physics::deltaTime});
        this->ejectedBulletSprites[i].setFillColor(sf::Color(255, 255, 255, std::max(0, this->ejectedBulletSprites[i].getFillColor().a - (int)(1000.0 * Physics::deltaTime))));
    }
}

void BulletMeter::ejectBullet(int bulletPos)
{
    this->ejectedBulletSprites.push_back(sf::RectangleShape(sf::Vector2f(BULLET_SPRITE_WIDTH + 2, BULLET_SPRITE_HEIGHT + 2)));
    this->ejectedBulletSprites[this->ejectedBulletSprites.size()-1].setPosition({this->position.x - 1 + bulletPos * (BULLET_SPRITE_WIDTH + BULLET_SPRITE_DISTANCE), this->position.y});
    this->ejectedBulletSprites[this->ejectedBulletSprites.size()-1].setFillColor(sf::Color::White);
}