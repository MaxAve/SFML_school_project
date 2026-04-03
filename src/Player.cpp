#include "../include/Player.hpp"

Player::Player(sf::RenderWindow& window) : inventory({10, 3},{1025.f, 700.f}, {(float)window.getSize().x / 2, (float)window.getSize().y / 2}) {
    this->sprite = sf::RectangleShape(sf::Vector2f(50.f, 100.f));
    this->sprite.setFillColor(sf::Color::Blue);
    this->sprite.setPosition({window.getSize().x / 2 - 25.f, window.getSize().y / 2 - 50.f});
    this->view = sf::View({800.f, 300.f}, {800.f, 600.f});
    this->speed = 300.0f;
    this->reloading = false;
    this->hitbox = Hitbox(this->sprite.getPosition(), this->sprite.getSize(), false);

    this->gunSprite = sf::RectangleShape(sf::Vector2f(70.f, 30.f));
    this->gunSprite.setFillColor(sf::Color::Red);
    this->gunSprite.setOrigin({0, this->gunSprite.getSize().y / 2});
    this->gunSprite.setPosition(sf::Vector2f(this->sprite.getPosition().x + this->sprite.getSize().x / 2, this->sprite.getPosition().y + this->sprite.getSize().y / 2));
}

void Player::update() {
    this->gunSprite.setPosition(sf::Vector2f(this->sprite.getPosition().x + this->sprite.getSize().x / 2, this->sprite.getPosition().y + this->sprite.getSize().y / 2));
    sf::Vector2i mousePos = Window::getMousePosFromCenter();
    this->gunSprite.setRotation(sf::degrees(std::atan2(mousePos.y, mousePos.x) / M_PI * 180.f));
}

void Player::draw(sf::RenderWindow& window) {
    this->view.setCenter({this->sprite.getPosition().x + 25.f, this->sprite.getPosition().y + 50.f});
    window.draw(this->sprite);
    window.draw(this->gunSprite);
}

void Player::setPosition(sf::Vector2f pos)
{
    this->sprite.setPosition(pos);
    this->hitbox.position = pos;
    this->hitbox.debugSprite.setPosition(this->hitbox.position);
}

void Player::move(sf::Vector2f delta)
{
    this->sprite.move(delta);

    for(int i = 0; i < Hitbox::solidHitboxPool.size(); i++)
    {
        Hitbox* hb = Hitbox::solidHitboxPool[i];
        if(this->sprite.getPosition().y <= (hb->position.y + hb->size.y) && (this->sprite.getPosition().y + this->hitbox.size.y) >= hb->position.y)
        {
            if(delta.x < 0 && this->sprite.getPosition().x < (hb->position.x + hb->size.x) && this->sprite.getPosition().x > hb->position.x)
                this->sprite.setPosition({hb->position.x + hb->size.x, this->sprite.getPosition().y});
            if(delta.x > 0 && (this->sprite.getPosition().x + this->hitbox.size.x) > hb->position.x && this->sprite.getPosition().x < (hb->position.x + hb->size.x))
                this->sprite.setPosition({hb->position.x - this->hitbox.size.x, this->sprite.getPosition().y});
        }
        if()
        {
            if(delta.y < 0 && this->sprite.getPosition().y < (hb->position.y + hb->size.y) && this->sprite.getPosition().y > hb->position.y)
                this->sprite.setPosition({this->sprite.getPosition().x, hb->position.y + hb->size.y});
        }
    }

    this->hitbox.position = this->sprite.getPosition();
    this->hitbox.debugSprite.setPosition(this->hitbox.position);
}