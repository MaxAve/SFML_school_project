#include "../include/player.h"

Player::Player(sf::RenderWindow& window)
{
    this->sprite = sf::RectangleShape(sf::Vector2f(50.f, 100.f));
    this->sprite.setFillColor(sf::Color::Green);
    this->sprite.setPosition(window.getSize().x / 2 - 25.f, window.getSize().y / 2 - 50.f);

    this->view = sf::View({800.f, 300.f}, {800.f, 600.f});

    this->speed = 300.0f;
}

void Player::draw(sf::RenderWindow &window)
{
    this->view.setCenter(this->sprite.getPosition().x + 25.f, this->sprite.getPosition().y + 50.f);
    window.draw(this->sprite);
}