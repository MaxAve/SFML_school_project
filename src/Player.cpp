#include "../include/Player.h"

Player::Player(sf::RenderWindow& window)
{
    this->sprite = sf::RectangleShape(sf::Vector2f(50.f, 100.f));
    this->sprite.setFillColor(sf::Color::Blue);
    this->sprite.setPosition({ window.getSize().x / 2 - 25.f, window.getSize().y / 2 - 50.f });
    this->view = sf::View({800.f, 300.f}, {800.f, 600.f});
    this->speed = 300.0f;

	this->gunSprite = sf::RectangleShape(sf::Vector2f(70.f, 30.f));
    this->gunSprite.setFillColor(sf::Color::Red);
	this->gunSprite.setOrigin({ 0, this->gunSprite.getSize().y/2 });
	this->gunSprite.setPosition(sf::Vector2f(this->sprite.getPosition().x + this->sprite.getSize().x/2, this->sprite.getPosition().y + this->sprite.getSize().y/2));
}

void Player::update()
{
	this->gunSprite.setPosition(sf::Vector2f(this->sprite.getPosition().x + this->sprite.getSize().x/2, this->sprite.getPosition().y + this->sprite.getSize().y/2));
	sf::Vector2i mousePos = getMousePosFromCenter();
	this->gunSprite.setRotation(sf::degrees(std::atan2(mousePos.y, mousePos.x) / M_PI * 180.f));
}

void Player::draw(sf::RenderWindow &window)
{
    this->view.setCenter({ this->sprite.getPosition().x + 25.f, this->sprite.getPosition().y + 50.f });
    window.draw(this->sprite);
	window.draw(this->gunSprite);
}
