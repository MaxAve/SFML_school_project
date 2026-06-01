#include "gui/BulletMeter.hpp"

BulletMeter::BulletMeter(sf::Vector2f position)
{
    this->position = position;
}

void BulletMeter::initSprites(Item* item)
{
    if (!item || !item->getData()) return;

    this->maxBullets = item->getData()->magCapacity;
    this->currentBullets = item->magSize; 
    
    this->sprites.clear();
    this->ejectedBulletSprites.clear(); 

    if (maxBullets <= 0) return;

    float width = BAR_WIDTH / static_cast<float>(maxBullets);

    for(int i = 0; i < this->maxBullets; i++)
    {
        this->sprites.push_back(sf::RectangleShape(sf::Vector2f(width, BULLET_SPRITE_HEIGHT)));
        this->sprites[i].setPosition({this->position.x + i * (width + BULLET_SPRITE_DISTANCE), this->position.y});
        
        if (i < (this->maxBullets - this->currentBullets)) {
            this->sprites[i].setFillColor(sf::Color::Transparent);
            this->sprites[i].setOutlineColor(sf::Color(80, 80, 80));
        } else {
            this->sprites[i].setFillColor(sf::Color::White);
            this->sprites[i].setOutlineColor(sf::Color(180, 180, 180));
        }
        
        this->sprites[i].setOutlineThickness(1);
    }
}

void BulletMeter::draw(sf::RenderWindow& window)
{
    for(int i = 0; i < this->maxBullets; i++)
    {
        window.draw(this->sprites[i]);
    }

    // for(int i = 0; i < this->ejectedBulletSprites.size(); i++)
    // {
    //     window.draw(this->ejectedBulletSprites[i]);
    // }
}

void BulletMeter::setCurrentBullets(int newCurrentBullets)
{
    for(int i = 0; i < (this->maxBullets - newCurrentBullets); i++)
    {
        this->sprites[i].setFillColor(sf::Color::Transparent);
        this->sprites[i].setOutlineColor(sf::Color::White);
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