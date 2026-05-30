#include "looting/DroppedItem.hpp"
#include "core/Physics.hpp"
#include <cmath>
#include <iostream>

#define ITEM_SIZE 60.f
#define FLOATING_AMPLITUDE 0.75f
#define FLOATING_FREQUENCY 0.5f
#define FLOATING_PERIOD 1.f / FLOATING_FREQUENCY
#define M_PI 3.14159265358979323846

#define LOG(message) std::cout << message << std::endl

std::vector<DroppedItem*> DroppedItem::pool;

DroppedItem::DroppedItem(Item* item_, sf::Vector2f pos_) : totalTime{0.f}, item{item_}, sprite(*(item->getTexture())) {
    LOG("DroppedItem(...)");

    sf::Vector2f textureSize = (sf::Vector2f)item->getTexture()->getSize();
    float targetSize = ITEM_SIZE;
    float scaleFactor = targetSize * 0.9f / std::max(textureSize.x, textureSize.y);
    sprite.setScale({scaleFactor, scaleFactor});
    sprite.setOrigin(sprite.getLocalBounds().getCenter());
    sprite.setPosition(pos_);

    LOG("DroppedItem(...)");
}

void DroppedItem::update() {
    // hover animation
    totalTime += Physics::deltaTime;

    if (totalTime >= FLOATING_PERIOD) {
        totalTime -= FLOATING_PERIOD;
    }

    float deltaS = FLOATING_AMPLITUDE * sin(2 * M_PI * FLOATING_FREQUENCY * totalTime);

    sprite.move({0.f, deltaS});
}

void DroppedItem::updateAll() {
    for (auto* it : pool) {
        it->update();
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