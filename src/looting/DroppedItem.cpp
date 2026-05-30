#include "looting/DroppedItem.hpp"
#include "core/Physics.hpp"
#include <cmath>
#include <iostream>

#define ITEM_SIZE 60.f
#define FLOATING_AMPLITUDE 0.75f
#define FLOATING_FREQUENCY 0.5f
#define FLOATING_PERIOD 1.f / FLOATING_FREQUENCY
#define M_PI 3.14159265358979323846

#define RANGE 125.f
#define COUNTDOWN 3.f

#define LOG(message) std::cout << message << std::endl

std::vector<DroppedItem*> DroppedItem::pool;

DroppedItem::DroppedItem(Item* item_, sf::Vector2f pos_) : pickupCountdown{COUNTDOWN}, totalTime{0.f}, item{item_}, sprite(*(item->getTexture())) {
    LOG("DroppedItem(...)");

    sf::Vector2f textureSize = (sf::Vector2f)item->getTexture()->getSize();
    float targetSize = ITEM_SIZE;
    float scaleFactor = targetSize * 0.9f / std::max(textureSize.x, textureSize.y);
    sprite.setScale({scaleFactor, scaleFactor});
    sprite.setOrigin(sprite.getLocalBounds().getCenter());
    sprite.setPosition(pos_);

    LOG("DroppedItem(...)");
}

bool DroppedItem::isPlayerInRange(sf::Vector2f playerPos) const {
    sf::Vector2f spritePos = sprite.getPosition();
    float distanceSq = (playerPos.x - spritePos.x) * (playerPos.x - spritePos.x) + (playerPos.y - spritePos.y) * (playerPos.y - spritePos.y);

    return distanceSq <= RANGE * RANGE;
}

bool DroppedItem::isReadyForPickup() const {
    return readyForPickup;
}

Item* DroppedItem::getItem() {
    return item;
}

Item* DroppedItem::popItem() {
    Item* ptr = nullptr;

    // extracting current object from the pool
    for (size_t i = 0; i < pool.size(); ++i) {
        if (pool[i] != this) {
            continue;
        }
        ptr = pool[i]->item;
        pool[i] = nullptr;
        std::swap(pool[i], pool.back());
        pool.resize(pool.size() - 1);
        break;
    }

    return ptr;
}

void DroppedItem::update(Player& player) {
    if (pickupCountdown > 0.f) {
        pickupCountdown -= Physics::deltaTime;
    }

    if (pickupCountdown <= 0.f && isPlayerInRange(player.hitbox.position)) {
        sf::Vector2i emptySlot = player.inventory.findEmptySlot();
        if (emptySlot != sf::Vector2i{-1, -1}) {
            player.inventory.setItem(sf::Vector2u{emptySlot.x, emptySlot.y}, popItem());
        }
    }
    // hover animation
    totalTime += Physics::deltaTime;

    if (totalTime >= FLOATING_PERIOD) {
        totalTime -= FLOATING_PERIOD;
    }

    float deltaS = FLOATING_AMPLITUDE * sin(2 * M_PI * FLOATING_FREQUENCY * totalTime);

    sprite.move({0.f, deltaS});
}

void DroppedItem::updateAll(Player& player) {
    for (auto* it : pool) {
        it->update(player);
    }
}

void DroppedItem::draw() {
    window.draw(sprite);
}

void DroppedItem::drawAll() {
    for (auto* it : pool) {
        it->draw();
    }
}