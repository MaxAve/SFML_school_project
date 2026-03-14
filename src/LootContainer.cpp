#include "../include/LootContainer.hpp"

LootContainer::LootContainer(sf::Vector2f pos, sf::Vector2f size, float _range)
    : range{_range} {

    box.setSize(size);
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setPosition(pos);
    box.setOutlineThickness(3.f);
    box.setFillColor(sf::Color::Cyan);
}

double LootContainer::getDistanceToSq(const sf::Vector2f coord) {
    const sf::Vector2f& chestPos = box.getPosition();
    return (chestPos.x - coord.x) * (chestPos.x - coord.x) + (chestPos.y - coord.y) * (chestPos.y - coord.y);
}

bool LootContainer::inRangeSq(double distance) {
    return (range * range) >= distance ? true : false;
}

void LootContainer::update(const Player& player) {
    if (inRangeSq(getDistanceToSq(player.sprite.getGlobalBounds().getCenter()))) {
        playerInRange = true;
    } else {
        playerInRange = false;
    }

    // highlight lootbox
    if (playerInRange) {
        box.setOutlineColor(sf::Color::Yellow);
    } else {
        box.setOutlineColor(sf::Color::Transparent);
    }
}

void LootContainer::draw() {
    window.draw(box);
}