#include "../include/PlayerhealthBar.hpp"

PlayerHealthBar::PlayerHealthBar(sf::Vector2f _size, unsigned _maxHealth = 100) : healthBar(_maxHealth), maxHealth(_maxHealth), text(Fonts::pixel) {
    text.setString("HP");

    healthBar.fullRect.setOrigin({0, 0});
    healthBar.healthRect.setOrigin({0, 0});
    healthBar.fullRect.setPosition({5, 5});
    healthBar.healthRect.setPosition({5, 5});

    healthBar.fullRect.setSize(_size);
    healthBar.healthRect.setSize(_size);

    healthBar.currentHealth = maxHealth;

    // set the position of text below the middle of the healthbar

    text.setOrigin({text.getLocalBounds().size.x / 2, 0.f});
    text.setPosition({healthBar.fullRect.getPosition().x + healthBar.fullRect.getGeometricCenter().x, healthBar.fullRect.getPosition().y + healthBar.fullRect.getSize().y + 1});
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
    healthBar.currentHealth += changeVal;

    if (healthBar.currentHealth > 100) {
        healthBar.currentHealth = 100;
    } else if (healthBar.currentHealth < 0) {
        healthBar.currentHealth = 0;
    }
}

void PlayerHealthBar::draw(sf::RenderWindow& window) {
    healthBar.draw(window);
    window.draw(text);
}