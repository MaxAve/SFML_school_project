#include "gui/InventoryInterface.hpp"
#include "resources/Fonts.hpp"
#include <iostream>
#define LOG(message) std::cout << message << std::endl


InventoryInterface::InventoryInterface(Inventory* _inventory, sf::Vector2f _size, sf::Vector2f _position) : title(Fonts::pixel, "Player", 20), amountOfCarriedItem(Fonts::pixel, "0", 20) {
    inventory = _inventory;
    carriedItem = nullptr;
    carriedItemSprite.reset();
    amountOfCarriedItem.setOrigin(amountOfCarriedItem.getLocalBounds().size);

    // setup background
    background.setSize({(float)defaultView.getSize().x, (float)defaultView.getSize().y});
    background.setFillColor(GuiParameters::backgroundColor);

    // setup foreground
    foreground.setSize(_size);
    foreground.setOrigin(foreground.getGeometricCenter());
    foreground.setPosition(_position);
    foreground.setFillColor(GuiParameters::foregroundColor);
    foreground.setOutlineThickness(GuiParameters::outlineThickness);
    foreground.setOutlineColor(GuiParameters::outlineColor);

    // create inventory slots
    sf::Vector2f topLeftCorner = foreground.getPosition() - _size / 2.f;
    padding = (_size - GuiParameters::slotSizeF * (sf::Vector2f)_inventory->getInventorySize()) / 2.f;
    title.setPosition({topLeftCorner.x + padding.x, topLeftCorner.y});
    std::vector<std::vector<InventorySlot>>* _inventorySlots = _inventory->getInventorySlots();

    inventorySlots.resize(_inventory->getInventorySize().x);
    for (size_t x = 0; x < inventorySlots.size(); x++) {
        inventorySlots[x].resize(_inventory->getInventorySize().y);
        for (size_t y = 0; y < inventorySlots[x].size(); y++) {
            inventorySlots[x][y].setSize(GuiParameters::slotSizeF);
            inventorySlots[x][y].setPosition({topLeftCorner.x + padding.x + GuiParameters::slotSizeF * x,
                                              topLeftCorner.y + padding.y + GuiParameters::slotSizeF * y});
            inventorySlots[x][y].setInventorySlot(&(*_inventorySlots)[x][y]);
        }
    }
}

void InventoryInterface::handleLMB(sf::Vector2f mousePos) {
    InventorySlotGui* targetSlot = nullptr;
    Item* selectedItem = nullptr;

    for (auto& row : inventorySlots) {
        for (auto& slot : row) {
            if (slot.isHovered()) {
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

    // if same Items
    if (targetSlot->getItem() && carriedItem && carriedItem->getType() == targetSlot->getItem()->getType()) {
        int diff = targetSlot->getItem()->addAmount(carriedItem->getAmount());
        if (diff > 0) {
            carriedItem->setAmount(diff);
        } else {
            carriedItem = nullptr;
            carriedItemSprite.reset();
        }

        return;
    }

    selectedItem = targetSlot->popItem();
    targetSlot->setItem(carriedItem);
    carriedItem = selectedItem;

    if (carriedItem) {
        setCarriedItemSprite(carriedItem->getTexture());
    } else {
        carriedItemSprite.reset();
    }
}

void InventoryInterface::setCarriedItemSprite(sf::Texture* tex) {
    carriedItemSprite.emplace(*(carriedItem->getTexture()));
    sf::Vector2f textureSize = static_cast<sf::Vector2f>(carriedItem->getTexture()->getSize());
    float factor = GuiParameters::slotSizeF * 0.9f / std::max(textureSize.x, textureSize.y);
    carriedItemSprite->setScale({factor, factor});
    carriedItemSprite->setOrigin(carriedItemSprite->getLocalBounds().getCenter());
}

void InventoryInterface::handleRMB(sf::Vector2f mousePos) {
    InventorySlotGui* targetSlot = nullptr;

    for (auto& row : inventorySlots) {
        for (auto& slot : row) {
            if (slot.isHovered()) {
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

    // split stack
    if (!carriedItem && targetSlot->getItem() && targetSlot->getItem()->getAmount() > 1) {
        carriedItem = new Item(*targetSlot->getItem()); // !
        size_t prevAmount = targetSlot->getItem()->getAmount();
        targetSlot->getItem()->setAmount(prevAmount / 2);
        carriedItem->setAmount(prevAmount - targetSlot->getItem()->getAmount());
        setCarriedItemSprite(carriedItem->getTexture());

        return;
    }

    // distribute one
    if (!carriedItem || (carriedItem->getAmount() <= 1)) {
        return;
    }
    if (!targetSlot->getItem()) {
        targetSlot->setItem(new Item(*carriedItem)); // !
        targetSlot->getItem()->setAmount(1);
        carriedItem->addAmount(-1);
    } else if (targetSlot->getItem()->getType() == carriedItem->getType()) { // if slot not empty
        if (targetSlot->getItem()->isFull()) {
            return;
        }
        targetSlot->getItem()->addAmount(1);
        carriedItem->addAmount(-1);

        if (!carriedItem->getAmount()) {
            carriedItem = nullptr;
            carriedItemSprite.reset();
        }
    }
}

void InventoryInterface::setPosition(sf::Vector2f newPosition) {
    foreground.setPosition(newPosition);

    sf::Vector2f topLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;
    title.setPosition({topLeftCorner.x + padding.x, topLeftCorner.y});

    for (size_t x = 0; x < inventorySlots.size(); x++) {
        for (size_t y = 0; y < inventorySlots[x].size(); y++) {
            inventorySlots[x][y].setPosition({topLeftCorner.x + padding.x + GuiParameters::slotSizeF * x,
                                              topLeftCorner.y + padding.y + GuiParameters::slotSizeF * y});
        }
    }
}

void InventoryInterface::resizeForeground(sf::Vector2f newSize) {
    foreground.setOrigin({0, 0});
    foreground.setSize(newSize);
    foreground.setOrigin(foreground.getLocalBounds().getCenter());
}

void InventoryInterface::resizeBackground(sf::Vector2f newSize) {
    background.setSize(newSize);
}

void InventoryInterface::update() {
    ItemLabel::visible = false;

    // checking if the slotGui corresponds with slot status
    // occurs because we set the item directly to the corresponding slot instead of slotGui
    // ? perhaps fix by solving the inconsistency of inventorySlot::setItem()
    for (auto& row : inventorySlots) {
        for (auto& slot : row) {
            if (!slot.getItem()) {
                if (slot.getSpriteTexture()) {
                    slot.setupItemSprite(nullptr);
                }
                continue;
            }

            if (slot.getSpriteTexture() != slot.getItem()->getTexture()) {
                slot.setupItemSprite(slot.getItem());
            }
        }
    }

    sf::Vector2f mousePos = (sf::Vector2f)Window::getMousePos();

    if (carriedItem) {
        carriedItemSprite->setPosition(mousePos);
        amountOfCarriedItem.setString(std::to_string(carriedItem->getAmount()));
        amountOfCarriedItem.setPosition({mousePos.x + GuiParameters::slotSizeF / 2.f - 13.f, mousePos.y + GuiParameters::slotSizeF / 2.f - 12.f});
    }

    for (auto& row : inventorySlots) {
        for (auto& slot : row) {
            slot.setHovered(false);
            slot.update();
        }
    }

    for (auto& row : inventorySlots) {
        for (auto& slot : row) {
            if (slot.getGlobalBounds().contains(mousePos)) {
                slot.setHovered(true);
                if (slot.getItem() != nullptr) {
                    ItemLabel::visible = true;
                    ItemLabel::update(slot.getItem()->getName(), slot.getItem()->getDescription());
                }
                return;
            }
        }
    }
}

void InventoryInterface::draw() {
    window.draw(background);
    window.draw(foreground);

    window.draw(title);

    for (size_t x = 0; x < inventorySlots.size(); x++) {
        for (size_t y = 0; y < inventorySlots[x].size(); y++) {
            inventorySlots[x][y].draw();
        }
    }

    if (carriedItemSprite) {
        window.draw(*(carriedItemSprite));
        if (carriedItem->getMaximalAmount() > 1)
            window.draw(amountOfCarriedItem);
    }
}