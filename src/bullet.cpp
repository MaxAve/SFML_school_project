#include "../include/bullet.h"

Bullet::Bullet(sf::Vector2f position, float speed, float direction)
{
    this->velocity = {std::cos(direction) * speed, std::sin(direction) * speed};

    this->sprite = sf::RectangleShape({Bullet::DEFAULT_SIZE, Bullet::DEFAULT_SIZE});
    this->sprite.setPosition(position);
    this->sprite.setFillColor(sf::Color::Yellow);
}

void Bullet::draw(sf::RenderWindow &window)
{
    window.draw(this->sprite);
}

void Bullet::update()
{
    this->sprite.move({this->velocity.x  * Physics::deltaTime, this->velocity.y * Physics::deltaTime});
}