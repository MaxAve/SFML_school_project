#include "../../include/gui/SharedInventory.hpp"

SharedInventory::SharedInventory() : first{nullptr}, second{nullptr} {}

SharedInventory::SharedInventory(Inventory* _first, Inventory* _second) : first{_first}, second{_second} {}

void SharedInventory::handleMousePress(sf::Vector2f mousePos) {
    sf::Vector2i slot;

    if (first && first->getGlobalBounds().contains(mousePos)) {
        slot = first->getSlotByMousePos(mousePos);
        if (slot.x == -1 || slot.y == -1) {
            return;
        }
        carriedItem = first->popBySlot(static_cast<sf::Vector2u>(slot), carriedItem);
    } else if (second && second->getGlobalBounds().contains(mousePos)) {
        slot = second->getSlotByMousePos(mousePos);
        if (slot.x == -1 || slot.y == -1) {
            return;
        }
        carriedItem = second->popBySlot(static_cast<sf::Vector2u>(slot), carriedItem);
    }

    if (carriedItem) {
        carriedItemSprite.emplace(*(carriedItem->getTexture()));
        sf::Vector2f textureSize = static_cast<sf::Vector2f>(carriedItem->getTexture()->getSize());
        float factor = Inventory::slotSizeF * 0.9f / std::max(textureSize.x, textureSize.y);
        carriedItemSprite->setScale({factor, factor});
        carriedItemSprite->setOrigin(carriedItemSprite->getLocalBounds().getCenter());
    } else {
        carriedItemSprite.reset();
    }
}

void SharedInventory::update() {
    if (carriedItemSprite) {
        (*carriedItemSprite).setPosition(static_cast<sf::Vector2f>(Window::getMousePos()));
    }

    first->update();
    second->update();
}

void SharedInventory::draw() {
    first->draw();
    second->draw();

    if (carriedItemSprite) {
        window.draw(*carriedItemSprite);
    }
}

void SharedInventory::setFirst(Inventory* _first) {
    first = _first;
}

void SharedInventory::setSecond(Inventory* _second) {
    second = _second;
}

Inventory* SharedInventory::getFirst() const {
    return first;
}

Inventory* SharedInventory::getSecond() const {
    return second;
}