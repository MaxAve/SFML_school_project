#include "gui/HealthBar.hpp"

HealthBar::HealthBar()
{}

HealthBar::HealthBar(int maxHealth)
{
    this->maxHealth = maxHealth;
    this->currentHealth = maxHealth;
    this->timeSinceHealthChangeAnimationBlink = 0.0f;
    this->blinkTimes = 0;
    this->healthChangeAnimationDeltaX = 0;

    this->healthRect = sf::RectangleShape(sf::Vector2f(HBAR_WIDTH, HBAR_HEIGHT));
    this->healthRect.setFillColor(sf::Color::Green);
    this->healthRect.setOrigin(sf::Vector2f(HBAR_WIDTH/2, HBAR_HEIGHT));

    this->fullRect = sf::RectangleShape(sf::Vector2f(HBAR_WIDTH, HBAR_HEIGHT));
    this->fullRect.setFillColor(sf::Color::Red);
    this->fullRect.setOrigin(sf::Vector2f(HBAR_WIDTH/2, HBAR_HEIGHT));

    this->healthChangeAnimation = sf::RectangleShape(sf::Vector2f(HBAR_WIDTH, HBAR_HEIGHT));
    this->healthChangeAnimation.setFillColor(sf::Color(255, 255, 255, 0));
    this->healthChangeAnimation.setOrigin(sf::Vector2f(0, HBAR_HEIGHT));
}

void HealthBar::draw(sf::RenderWindow& window)
{
    if(this->blinkTimes > 0)
    {
        this->timeSinceHealthChangeAnimationBlink += Physics::deltaTime;

        if(this->timeSinceHealthChangeAnimationBlink > 0.05f)
        {
            if(this->healthChangeAnimation.getFillColor().a == 0)
            {
                this->healthChangeAnimation.setFillColor(sf::Color(255, 255, 255, 255));
                this->blinkTimes--;
                if(this->blinkTimes <= 0)
                    this->healthChangeAnimation.setFillColor(sf::Color(255, 255, 255, 0));
                }
            else
                this->healthChangeAnimation.setFillColor(sf::Color(255, 255, 255, 0));
            this->timeSinceHealthChangeAnimationBlink = 0;
        }
    }

    window.draw(this->fullRect);
    window.draw(this->healthRect);
    window.draw(this->healthChangeAnimation);
}

void HealthBar::setHealth(int newHealth)
{
    int deltaH = std::abs(this->currentHealth - newHealth);
    this->currentHealth = std::max(0, std::min(newHealth, this->maxHealth));
    this->healthRect.setSize(sf::Vector2f((float)this->currentHealth / (float)this->maxHealth * HBAR_WIDTH, HBAR_HEIGHT));
    this->healthChangeAnimation.setSize(sf::Vector2f((float)deltaH / (float)this->maxHealth * HBAR_WIDTH, HBAR_HEIGHT));
    this->healthChangeAnimationDeltaX = (float)this->currentHealth / (float)this->maxHealth * HBAR_WIDTH;
    this->blinkTimes = 5;
}

void HealthBar::setPosition(sf::Vector2f pos)
{
    this->healthRect.setPosition(pos);
    this->fullRect.setPosition(pos);
    this->healthChangeAnimation.setPosition({pos.x + this->healthChangeAnimationDeltaX - HBAR_WIDTH/2, pos.y});
}