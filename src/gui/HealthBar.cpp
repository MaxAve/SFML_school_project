#include "../../include/gui/HealthBar.hpp"

HealthBar::HealthBar()
{}

HealthBar::HealthBar(int maxHealth)
{
    this->maxHealth = maxHealth;
    this->currentHealth = maxHealth;

    this->healthRect = sf::RectangleShape(sf::Vector2f(HBAR_WIDTH, HBAR_HEIGHT));
    this->healthRect.setFillColor(sf::Color::Green);
    this->healthRect.setOrigin(sf::Vector2f(HBAR_WIDTH/2, HBAR_HEIGHT));
    this->fullRect = sf::RectangleShape(sf::Vector2f(HBAR_WIDTH, HBAR_HEIGHT));
    this->fullRect.setFillColor(sf::Color::Red);
    this->fullRect.setOrigin(sf::Vector2f(HBAR_WIDTH/2, HBAR_HEIGHT));
}

void HealthBar::draw(sf::RenderWindow& window)
{
    window.draw(this->fullRect);
    window.draw(this->healthRect);
}

void HealthBar::setHealth(int newHealth)
{
    this->currentHealth = std::max(0, std::min(newHealth, this->maxHealth));
    this->healthRect.setSize(sf::Vector2f((float)this->currentHealth / (float)this->maxHealth * HBAR_WIDTH, HBAR_HEIGHT));
}

void HealthBar::setPosition(sf::Vector2f pos)
{
    this->healthRect.setPosition(pos);
    this->fullRect.setPosition(pos);
}