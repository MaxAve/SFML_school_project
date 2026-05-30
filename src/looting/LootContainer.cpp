#include "looting/LootContainer.hpp"
#include "core/Physics.hpp"
#include "resources/Textures.hpp"

#define FLOATING_AMPLITUDE 7.5f
#define FLOATING_FREQUENCY 0.5f
#define FLOATING_PERIOD 1.f / FLOATING_FREQUENCY
#define M_PI 3.14159265358979323846

float LootContainer::promptDistanceFromContainer = 8.f;

std::vector<LootContainer*> LootContainer::pool;

LootContainer::LootContainer(size_t _type, sf::Vector2f _pos, sf::Vector2f _size, float _range)
    : type{_type}, range{_range}, inventory({10, 3}, "Lootbox"), prompt(*Textures::get("gui_hud/q_key_prompt")) {

    box.setSize(_size);
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setPosition(_pos);
    box.setOutlineThickness(3.f);
    box.setFillColor(sf::Color::Cyan);

    prompt.setScale({3.f, 3.f});
    prompt.setOrigin(prompt.getLocalBounds().getCenter() + sf::Vector2f{0, prompt.getLocalBounds().size.y / 2});
    prompt.setPosition(box.getPosition() - sf::Vector2f{0, box.getGlobalBounds().size.y / 2 + promptDistanceFromContainer});
    promptDefPos = prompt.getPosition();
}

double LootContainer::getDistanceToSq(const sf::Vector2f coord) {
    const sf::Vector2f& chestPos = box.getPosition();
    return (chestPos.x - coord.x) * (chestPos.x - coord.x) + (chestPos.y - coord.y) * (chestPos.y - coord.y);
}

double LootContainer::getDistanceTo(const sf::Vector2f coord) {
    return sqrt(getDistanceToSq(coord));
}

std::vector<std::vector<InventorySlot>>* LootContainer::getInventorySlots() {
    return inventory.getInventorySlots();
}

const sf::Vector2f LootContainer::getPosition() const {
    return box.getPosition();
}

Inventory* LootContainer::getInventory() {
    return &inventory;
}

bool LootContainer::isPlayerInRange() const {
    return playerInRange;
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

    // prompt animation
    if (playerInRange) {
        totalTime += Physics::deltaTime;

        if (totalTime >= FLOATING_PERIOD) {
            totalTime -= FLOATING_PERIOD;
        }

        float deltaS = FLOATING_AMPLITUDE * sin(2 * M_PI * FLOATING_FREQUENCY * totalTime);

        prompt.setPosition(promptDefPos + sf::Vector2f{0.f, deltaS});
    }
}

void LootContainer::updateAll(const Player& player) {
    for (auto* it : pool) {
        it->update(player);
    }
}

void LootContainer::draw() {
    window.draw(box);

    if (playerInRange) {
        window.draw(prompt);
    }
}

void LootContainer::drawAll() {
    for (auto& lc : pool) {
        lc->draw();
    }
}