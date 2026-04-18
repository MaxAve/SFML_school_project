#include "gui/SharedInventoryInterface.hpp"
#include "gui/InventoryInterface.hpp"
#include "resources/Fonts.hpp"

#include <iostream>
#define LOG(message) std::cout << message << std::endl

const sf::Color SharedInventoryInterface::stdBackgroundColor(20, 20, 25, 150);
const sf::Color SharedInventoryInterface::stdForegroundColor(45, 50, 55, 230);
const float SharedInventoryInterface::stdOutlineThickness = 2;
const sf::Color SharedInventoryInterface::stdOutlineColor(100, 105, 115);
unsigned SharedInventoryInterface::slotSizeU = 95u;
float SharedInventoryInterface::slotSizeF = static_cast<float>(slotSizeU);

SharedInventoryInterface::SharedInventoryInterface(sf::Vector2f _size, sf::Vector2f _position, Inventory* _mainInventory, Inventory* _otherInventory) : mainTitle(Fonts::pixel, "", 20), otherTitle(Fonts::pixel, "", 20), mainInventory{_mainInventory}, otherInventory{_otherInventory}, amountOfCarriedItem(Fonts::pixel, "0", 20) {
    carriedItem = nullptr;
    carriedItemSprite.reset();
    amountOfCarriedItem.setOrigin(amountOfCarriedItem.getLocalBounds().size);

    // setup background
    background.setSize({(float)defaultView.getSize().x, (float)defaultView.getSize().y});
    background.setFillColor(stdBackgroundColor);

    // setup foreground
    foreground.setSize(_size);
    foreground.setOrigin(foreground.getGeometricCenter());
    foreground.setPosition(_position);
    foreground.setFillColor(stdForegroundColor);
    foreground.setOutlineThickness(stdOutlineThickness);
    foreground.setOutlineColor(stdOutlineColor);

    setMainInventory(mainInventory);
    setOtherInventory(otherInventory);
}

InventorySlotGui* SharedInventoryInterface::findHoveredSlot() {
    for (auto& row : otherInventorySlots) {
        for (auto& slot : row) {
            if (slot.isHovered()) {
                return &slot;
            }
        }
    }

    for (auto& row : mainInventorySlots) {
        for (auto& slot : row) {
            if (slot.isHovered()) {
                return &slot;
            }
        }
    }

    return nullptr;
}

void SharedInventoryInterface::handleMousePress(sf::Vector2f mousePos) {
    InventorySlotGui* targetSlot = findHoveredSlot();
    Item* selectedItem = nullptr;

    if (!targetSlot) {
        return;
    }

    // if same Items
    if (targetSlot->getItem() && carriedItem && carriedItem->getType() == targetSlot->getItem()->getType()) {
        int diff = targetSlot->getItem()->addAmount(carriedItem->getAmount());
        if (diff > 0) {
            carriedItem->setAmount(diff);
            amountOfCarriedItem.setString(std::to_string(diff));
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
        carriedItemSprite.emplace(*(carriedItem->getTexture()));
        sf::Vector2f textureSize = static_cast<sf::Vector2f>(carriedItem->getTexture()->getSize());
        float factor = slotSizeF * 0.9f / std::max(textureSize.x, textureSize.y);
        carriedItemSprite->setScale({factor, factor});
        carriedItemSprite->setOrigin(carriedItemSprite->getLocalBounds().getCenter());
        amountOfCarriedItem.setString(std::to_string(carriedItem->getAmount()));
    } else {
        carriedItemSprite.reset();
    }
}

void SharedInventoryInterface::resizeForeground(sf::Vector2f newSize) {
    foreground.setOrigin({0, 0});
    foreground.setSize(newSize);
    foreground.setOrigin(foreground.getLocalBounds().getCenter());
}

void SharedInventoryInterface::resizeBackground(sf::Vector2f newSize) {
    background.setSize(newSize);
}

void SharedInventoryInterface::setOtherInventory(Inventory* inventory) {
    otherInventory = inventory;

    if (!inventory) {
        otherTitle.setString("");
        otherInventorySlots.clear();

        return;
    }

    otherTitle.setString(inventory->getTitle());
    sf::Vector2u invSize = inventory->getInventorySize();

    sf::Vector2f upperLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;
    paddingU = ((foreground.getSize() - sf::Vector2f{0, foreground.getSize().y / 2.f}) - static_cast<sf::Vector2f>(invSize) * slotSizeF) / 2.f;
    std::vector<std::vector<InventorySlot>>* _inventorySlots = inventory->getInventorySlots();

    otherTitle.setPosition({upperLeftCorner.x + paddingU.x, upperLeftCorner.y});

    otherInventorySlots.resize(invSize.x);
    for (size_t x = 0; x < invSize.x; x++) {
        otherInventorySlots[x].resize(invSize.y);
        for (size_t y = 0; y < invSize.y; y++) {
            otherInventorySlots[x][y].setSize(SharedInventoryInterface::slotSizeF);
            otherInventorySlots[x][y].setPosition({upperLeftCorner.x + paddingU.x + slotSizeF * x,
                                                   upperLeftCorner.y + paddingU.y + slotSizeF * y});
            otherInventorySlots[x][y].setInventorySlot(&(*_inventorySlots)[x][y]);
        }
    }
}

void SharedInventoryInterface::setMainInventory(Inventory* inventory) {
    mainInventory = inventory;

    if (!inventory) {
        mainTitle.setString("");
        mainInventorySlots.clear();

        return;
    }

    mainTitle.setString(inventory->getTitle());
    sf::Vector2u invSize = inventory->getInventorySize();

    sf::Vector2f halfSize = foreground.getSize() / 2.f;
    sf::Vector2f middleLeftCorner = {foreground.getPosition().x - halfSize.x, foreground.getPosition().y};
    paddingL = ((foreground.getSize() - sf::Vector2f{0, halfSize.y}) - static_cast<sf::Vector2f>(invSize) * slotSizeF) / 2.f;
    std::vector<std::vector<InventorySlot>>* _inventorySlots = inventory->getInventorySlots();

    mainTitle.setPosition({middleLeftCorner.x + paddingL.x, middleLeftCorner.y});

    mainInventorySlots.resize(invSize.x);
    for (size_t x = 0; x < invSize.x; x++) {
        mainInventorySlots[x].resize(invSize.y);
        for (size_t y = 0; y < invSize.y; y++) {
            mainInventorySlots[x][y].setSize(SharedInventoryInterface::slotSizeF);
            mainInventorySlots[x][y].setPosition({middleLeftCorner.x + paddingL.x + slotSizeF * x,
                                                  middleLeftCorner.y + paddingL.y + slotSizeF * y});
            mainInventorySlots[x][y].setInventorySlot(&(*_inventorySlots)[x][y]);
        }
    }
}

Inventory* SharedInventoryInterface::getOtherInventory() {
    return otherInventory;
}

Inventory* SharedInventoryInterface::getMainInventory() {
    return mainInventory;
}

void SharedInventoryInterface::setPosition(sf::Vector2f newPosition) {
    foreground.setPosition(newPosition);

    if (otherInventory) {
        sf::Vector2f upperLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;

        otherTitle.setPosition({upperLeftCorner.x + paddingU.x, upperLeftCorner.y});

        for (size_t x = 0; x < otherInventorySlots.size(); x++) {
            for (size_t y = 0; y < otherInventorySlots[x].size(); y++) {
                otherInventorySlots[x][y].setPosition({upperLeftCorner.x + paddingU.x + slotSizeF * x,
                                                       upperLeftCorner.y + paddingU.y + slotSizeF * y});
            }
        }
    }

    if (mainInventory) {
        sf::Vector2f halfSize = foreground.getSize() / 2.f;
        sf::Vector2f middleLeftCorner = {foreground.getPosition().x - halfSize.x, foreground.getPosition().y};

        mainTitle.setPosition({middleLeftCorner.x + paddingL.x, middleLeftCorner.y});

        for (size_t x = 0; x < mainInventorySlots.size(); x++) {
            for (size_t y = 0; y < mainInventorySlots[x].size(); y++) {
                mainInventorySlots[x][y].setPosition({middleLeftCorner.x + paddingU.x + slotSizeF * x,
                                                      middleLeftCorner.y + paddingU.y + slotSizeF * y});
            }
        }
    }
}

void SharedInventoryInterface::update() {
    ItemLabel::visible = false;

    // checking if the slotGui corresponds with slot status
    // occurs because we set the item directly to the corresponding slot instead of slotGui
    // ? perhaps fix by solving the inconsistency of inventorySlot::setItem()
    for (auto& row : mainInventorySlots) {
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

    for (auto& row : otherInventorySlots) {
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
        amountOfCarriedItem.setPosition({mousePos.x + slotSizeF/2.f - 4.f, mousePos.y + slotSizeF/2.f - 12.f});
    }

    for (auto& row : mainInventorySlots) {
        for (auto& slot : row) {
            slot.setHovered(false);
            slot.update();
        }
    }

    for (auto& row : otherInventorySlots) {
        for (auto& slot : row) {
            slot.setHovered(false);
            slot.update();
        }
    }

    for (auto& row : mainInventorySlots) {
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

    for (auto& row : otherInventorySlots) {
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

void SharedInventoryInterface::draw() {
    window.draw(background);
    window.draw(foreground);

    window.draw(otherTitle);
    window.draw(mainTitle);

    for (const auto& row : otherInventorySlots) {
        for (const auto& slot : row) {
            slot.draw();
        }
    }

    for (const auto& row : mainInventorySlots) {
        for (const auto& slot : row) {
            slot.draw();
        }
    }

    if (carriedItemSprite) {
        window.draw(*(carriedItemSprite));
        window.draw(amountOfCarriedItem);
    }
}