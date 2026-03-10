#include "../include/LootContainer.hpp"

LootContainer::LootContainer(sf::Vector2f pos, sf::Vector2f size, float _range)
    : range{_range} {

    box.setSize(size);
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setPosition(pos);
    box.setOutlineThickness(3.f);
    box.setFillColor(sf::Color::Cyan);
}

void LootContainer::update(const Player& player) {
    const sf::Vector2f& chestPos = box.getPosition();
    const sf::Vector2f& playerPos = player.sprite.getGlobalBounds().getCenter();

    // inefficient way:
    // float distance = sqrt(pow(chestPos.x - playerPos.x, 2) + pow(chestPos.y - playerPos.y, 2)); 
    float distance = (chestPos.x - playerPos.x) * (chestPos.x - playerPos.x) + (chestPos.y - playerPos.y) * (chestPos.y - playerPos.y);

    if (distance < (range * range)) {
        box.setOutlineColor(sf::Color::Yellow);
    } else {
        box.setOutlineColor(sf::Color::Transparent);
    }
}

void LootContainer::draw() {
    window.draw(box);
}