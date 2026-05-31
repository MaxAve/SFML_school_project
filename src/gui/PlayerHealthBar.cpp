#include "gui/PlayerHealthBar.hpp"

PlayerHealthBar::PlayerHealthBar(sf::Vector2f _size, unsigned _maxHealth = 100) : healthBar(_maxHealth), maxHealth(_maxHealth) {
    healthBar.fullRect.setOrigin({0, 0});
    healthBar.healthRect.setOrigin({0, 0});
    healthBar.fullRect.setPosition({5, 5});
    healthBar.healthRect.setPosition({5, 5});

    size = _size;
    healthBar.fullRect.setSize(_size);
    healthBar.healthRect.setSize(_size);

    healthBar.currentHealth = maxHealth;
}

unsigned PlayerHealthBar::getHealth() const {
    return healthBar.currentHealth;
}

void PlayerHealthBar::setHealth(unsigned newHealth) {
    if (newHealth > 100) {
        newHealth = 100;
    } else if (newHealth < 0) {
        newHealth = 0;
    }

    healthBar.currentHealth = newHealth;
}

void PlayerHealthBar::changeHealth(int changeVal) {
    int newHealth =healthBar.currentHealth + changeVal;

    if (newHealth > 100) {
        newHealth = 100;
    } else if (newHealth < 0) {
        newHealth = 0;
    }

    int deltaH = std::abs(healthBar.currentHealth - newHealth);
    healthBar.currentHealth = std::max(0, std::min(newHealth, healthBar.maxHealth));
    healthBar.healthRect.setSize(sf::Vector2f((float)healthBar.currentHealth / (float)healthBar.maxHealth * size.x, size.y));
    healthBar.healthChangeAnimation.setSize(sf::Vector2f((float)deltaH / (float)healthBar.maxHealth * size.x, size.y));
    healthBar.healthChangeAnimationDeltaX = (float)healthBar.currentHealth / (float)healthBar.maxHealth * size.x;
    healthBar.blinkTimes = 5;
}

void PlayerHealthBar::draw(sf::RenderWindow& window) {
    healthBar.draw(window);
}
