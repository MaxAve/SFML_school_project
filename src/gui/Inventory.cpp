#include "../../include/gui/Inventory.hpp"

const sf::Color Inventory::stdBackgroundColor(20, 20, 25, 150);
const sf::Color Inventory::stdForegroundColor(45, 50, 55, 230);
const float Inventory::stdOutlineThickness = 2;
const sf::Color Inventory::stdOutlineColor(100, 105, 115);
unsigned Inventory::slotSizeU = 95u;
float Inventory::slotSizeF = static_cast<float>(slotSizeU);

Inventory::Inventory(sf::Vector2u inventorySize, const sf::Vector2f& size, const sf::Vector2f& position) {
    carriedItem = nullptr;
    carriedItemSprite.reset();

    // setup background
    background.setSize({(float)defaultView.getSize().x, (float)defaultView.getSize().y});
    background.setFillColor(stdBackgroundColor);

    // setup foreground
    foreground.setSize(size);
    foreground.setOrigin(foreground.getGeometricCenter());
    foreground.setPosition(position);
    foreground.setFillColor(stdForegroundColor);
    foreground.setOutlineThickness(stdOutlineThickness);
    foreground.setOutlineColor(stdOutlineColor);

    // create inventory slots
    sf::Vector2f topLeftCorner = foreground.getPosition() - size / 2.f;
    padding = (size - slotSizeF * (sf::Vector2f)inventorySize) / 2.f;

    inventorySlots.resize(inventorySize.x);
    for (size_t x = 0; x < inventorySlots.size(); x++) {
        inventorySlots[x].resize(inventorySize.y);
        for (size_t y = 0; y < inventorySlots[x].size(); y++) {
            inventorySlots[x][y].setSize(slotSizeF);
            inventorySlots[x][y].setPosition({topLeftCorner.x + padding.x + slotSizeF * x,
                                              topLeftCorner.y + padding.y + slotSizeF * y});
        }
    }
}

void Inventory::handleMousePress(sf::Vector2f mousePos) {
    InventorySlot* targetSlot = nullptr;
    Item* selectedItem = nullptr;

    for (auto& row : inventorySlots) {
        for (auto& slot : row) {
            if (slot.getGlobalBounds().contains(mousePos)) {
                targetSlot = &slot;
                break;
            }
        }
        if (targetSlot) {
            break;
        }
    }
    
    if (!targetSlot) {
        return;
    }

    selectedItem = targetSlot->popItem();
    targetSlot->setItem(carriedItem);
    carriedItem = selectedItem;

    if (carriedItem) {
        carriedItemSprite.emplace(*(carriedItem->getTexture()));
        sf::Vector2f textureSize = static_cast<sf::Vector2f>(carriedItem->getTexture()->getSize());
        float factor = slotSizeF * 0.9f / std::max(textureSize.x, textureSize.y);
        carriedItemSprite->setScale({factor, factor});
        carriedItemSprite->setOrigin(carriedItemSprite->getLocalBounds().getCenter());
    } else {
        carriedItemSprite.reset();
    }
}

void Inventory::setPosition(sf::Vector2f newPosition) {
    foreground.setPosition(newPosition);

    sf::Vector2f topLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;
    for (size_t x = 0; x < inventorySlots.size(); x++) {
        for (size_t y = 0; y < inventorySlots[x].size(); y++) {
            inventorySlots[x][y].setPosition({topLeftCorner.x + padding.x + slotSizeF * x,
                                              topLeftCorner.y + padding.y + slotSizeF * y});
        }
    }
}

sf::Vector2u Inventory::getInventorySize() const {
    return inventorySize;
}

std::vector<std::vector<InventorySlot>>* Inventory::getInventorySlots() {
    return &inventorySlots;
}

void Inventory::setItem(sf::Vector2u slot, Item* item) {
    inventorySlots[slot.x][slot.y].setItem(item);
}

void Inventory::resizeForeground(sf::Vector2f newSize) {
    foreground.setOrigin({0, 0});
    foreground.setSize(newSize);
    foreground.setOrigin(foreground.getLocalBounds().getCenter());
}

void Inventory::resizeBackground(sf::Vector2f newSize) {
    background.setSize(newSize);
}

void Inventory::update() {
    sf::Vector2f mousePos = (sf::Vector2f)Window::getMousePos();

    if (carriedItem) {
        carriedItemSprite->setPosition(mousePos);
    }

    for (auto& row : inventorySlots) {
        for (auto& slot : row) {
            slot.setHovered(false);
        }
    }

    for (auto& row : inventorySlots) {
        for (auto& slot : row) {
            if (slot.getGlobalBounds().contains(mousePos)) {
                slot.setHovered(true);
                return;
            }
        }
    }
}

void Inventory::draw() {
    window.draw(background);
    window.draw(foreground);

    for (size_t x = 0; x < inventorySlots.size(); x++) {
        for (size_t y = 0; y < inventorySlots[x].size(); y++) {
            inventorySlots[x][y].draw();
        }
    }

    if (carriedItemSprite) {
        window.draw(*(carriedItemSprite));
    }
}