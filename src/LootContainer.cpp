#include "../include/LootContainer.hpp"

LootContainer::LootContainer(sf::Vector2u inventorySize, sf::Vector2f pos, sf::Vector2f size, float _range)
    : range{_range}, container{inventorySize, {1025.f, 350.f}, {(float)window.getSize().x / 2, (float)window.getSize().y * 0.75f / 2}} {

    box.setSize(size);
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setPosition(pos);
    box.setOutlineThickness(3.f);
    box.setFillColor(sf::Color::Cyan);
}

float LootContainer::calcDistanceSq(sf::Vector2f coord) const {
    const sf::Vector2f& chestPos = box.getPosition();

    // inefficient way:
    // float distance = sqrt(pow(chestPos.x - playerPos.x, 2) + pow(chestPos.y - playerPos.y, 2));
    return (chestPos.x - coord.x) * (chestPos.x - coord.x) + (chestPos.y - coord.y) * (chestPos.y - coord.y);
}

bool LootContainer::inRange(sf::Vector2f coord) const {
    return calcDistanceSq(coord) < (range * range);
}

void LootContainer::update(const Player& player, SharedInventory& sharedInventory) {
    if (inRange(player.sprite.getGlobalBounds().getCenter())) {
        box.setOutlineColor(sf::Color::Yellow);
        sharedInventory.setSecond(&container);
    } else {
        box.setOutlineColor(sf::Color::Transparent);
    }
}

void LootContainer::draw() {
    window.draw(box);
}