#include "gui/SharedInventoryInterface.hpp"
#include "gui/InventoryInterface.hpp"
#include "resources/Fonts.hpp"

#include <iostream>
#define LOG(message) std::cout << message << std::endl

SharedInventoryInterface::SharedInventoryInterface(sf::Vector2f _size, sf::Vector2f _position, Inventory* _mainInventory, Hotbar* _hotbar, Inventory* _otherInventory) : mainTitle(Fonts::pixel, "", 20), otherTitle(Fonts::pixel, "", 20), amountOfCarriedItem(Fonts::pixel, "0", 20) {
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

    setMainInventory(_mainInventory);
    setHotbar(_hotbar);
    setOtherInventory(_otherInventory);
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

    for (auto& slot : *hotbarGui.getGuiSlots()) {
        if (slot.isHovered()) {
            return &slot;
        }
    }

    return nullptr;
}

void SharedInventoryInterface::setCarriedItemSprite(sf::Texture* tex) {
    carriedItemSprite.emplace(*(carriedItem->getTexture()));
    sf::Vector2f textureSize = static_cast<sf::Vector2f>(carriedItem->getTexture()->getSize());
    float factor = GuiParameters::slotSizeF * 0.9f / std::max(textureSize.x, textureSize.y);
    carriedItemSprite->setScale({factor, factor});
    carriedItemSprite->setOrigin(carriedItemSprite->getLocalBounds().getCenter());
}

void SharedInventoryInterface::handleLMB(sf::Vector2f mousePos) {
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
        amountOfCarriedItem.setString(std::to_string(carriedItem->getAmount()));
    } else {
        carriedItemSprite.reset();
    }
}

void SharedInventoryInterface::handleRMB(sf::Vector2f mousePos) {
    InventorySlotGui* targetSlot = findHoveredSlot();

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

    // space for both inventories (not including the hotbar)
    sf::Vector2f inventorySpace = foreground.getSize() - sf::Vector2f{0, GuiParameters::slotSizeF + 0.f};

    sf::Vector2f upperLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;
    paddingU = ((inventorySpace - sf::Vector2f{0, inventorySpace.y / 2.f}) - static_cast<sf::Vector2f>(invSize) * GuiParameters::slotSizeF) / 2.f;
    std::vector<std::vector<InventorySlot>>* _inventorySlots = inventory->getInventorySlots();

    otherTitle.setPosition({upperLeftCorner.x + paddingU.x, upperLeftCorner.y});

    otherInventorySlots.resize(invSize.x);
    for (size_t x = 0; x < invSize.x; x++) {
        otherInventorySlots[x].resize(invSize.y);
        for (size_t y = 0; y < invSize.y; y++) {
            otherInventorySlots[x][y].setSize(GuiParameters::slotSizeF);
            otherInventorySlots[x][y].setPosition({upperLeftCorner.x + paddingU.x + GuiParameters::slotSizeF * x,
                                                   upperLeftCorner.y + paddingU.y + GuiParameters::slotSizeF * y});
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

    // space for both inventories (not including the hotbar)
    sf::Vector2f inventorySpace = foreground.getSize() - sf::Vector2f{0, GuiParameters::slotSizeF + 0.f};

    sf::Vector2f upperLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;
    // place main inventory in the bottom half of the available inventorySpace
    sf::Vector2f middleLeftCorner = {upperLeftCorner.x, upperLeftCorner.y + inventorySpace.y / 2.f};
    paddingL = ((inventorySpace - sf::Vector2f{0, inventorySpace.y / 2.f}) - static_cast<sf::Vector2f>(invSize) * GuiParameters::slotSizeF) / 2.f;
    std::vector<std::vector<InventorySlot>>* _inventorySlots = inventory->getInventorySlots();

    mainTitle.setPosition({middleLeftCorner.x + paddingL.x, middleLeftCorner.y});

    mainInventorySlots.resize(invSize.x);
    for (size_t x = 0; x < invSize.x; x++) {
        mainInventorySlots[x].resize(invSize.y);
        for (size_t y = 0; y < invSize.y; y++) {
            mainInventorySlots[x][y].setSize(GuiParameters::slotSizeF);
            mainInventorySlots[x][y].setPosition({middleLeftCorner.x + paddingL.x + GuiParameters::slotSizeF * x,
                                                  middleLeftCorner.y + paddingL.y + GuiParameters::slotSizeF * y});
            mainInventorySlots[x][y].setInventorySlot(&(*_inventorySlots)[x][y]);
        }
    }
}

void SharedInventoryInterface::setHotbar(Hotbar* _hotbar) {
    hotbar = _hotbar;
    hotbarGui.setHotbar(hotbar);

    if (!hotbar) {
        return;
    }

    sf::Vector2f upperLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;
    float hotbarWidth = hotbar->getSize() * GuiParameters::slotSizeF;
    sf::Vector2f hotbarPosition = {upperLeftCorner.x + (foreground.getSize().x - hotbarWidth) / 2.f,
                                   upperLeftCorner.y + foreground.getSize().y - GuiParameters::slotSizeF - 7.5f};
    hotbarGui.setPosition(hotbarPosition);
}

Inventory* SharedInventoryInterface::getOtherInventory() {
    return otherInventory;
}

Inventory* SharedInventoryInterface::getMainInventory() {
    return mainInventory;
}

void SharedInventoryInterface::setPosition(sf::Vector2f newPosition) {
    // space for both inventories (not including the hotbar)
    sf::Vector2f inventorySpace = foreground.getSize() - sf::Vector2f{0, GuiParameters::slotSizeF + 50.f};
    foreground.setPosition(newPosition);

    if (otherInventory) {
        sf::Vector2f upperLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;

        otherTitle.setPosition({upperLeftCorner.x + paddingU.x, upperLeftCorner.y});

        for (size_t x = 0; x < otherInventorySlots.size(); x++) {
            for (size_t y = 0; y < otherInventorySlots[x].size(); y++) {
                otherInventorySlots[x][y].setPosition({upperLeftCorner.x + paddingU.x + GuiParameters::slotSizeF * x,
                                                       upperLeftCorner.y + paddingU.y + GuiParameters::slotSizeF * y});
            }
        }
    }

    if (mainInventory) {
        sf::Vector2f upperLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;
        // origin for main inventory: start at left edge, halfway down the inventorySpace
        sf::Vector2f middleLeftCorner = {upperLeftCorner.x, upperLeftCorner.y + inventorySpace.y / 2.f};

        mainTitle.setPosition({middleLeftCorner.x + paddingL.x, middleLeftCorner.y});

        for (size_t x = 0; x < mainInventorySlots.size(); x++) {
            for (size_t y = 0; y < mainInventorySlots[x].size(); y++) {
                mainInventorySlots[x][y].setPosition({middleLeftCorner.x + paddingL.x + GuiParameters::slotSizeF * x,
                                                      middleLeftCorner.y + paddingL.y + GuiParameters::slotSizeF * y});
            }
        }
    }

    if (hotbar) {
        // reposition hotbar to remain centered at the bottom of the foreground
        sf::Vector2f upperLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;
        float hotbarWidth = hotbar->getSize() * GuiParameters::slotSizeF;
        sf::Vector2f hotbarPosition = {upperLeftCorner.x + (foreground.getSize().x - hotbarWidth) / 2.f,
                                       upperLeftCorner.y + foreground.getSize().y - GuiParameters::slotSizeF - 7.5f};
        hotbarGui.setPosition(hotbarPosition);
    }
}

void SharedInventoryInterface::update() {
    ItemLabel::visible = false;

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
        amountOfCarriedItem.setString(std::to_string(carriedItem->getAmount()));
        amountOfCarriedItem.setPosition({mousePos.x + GuiParameters::slotSizeF / 2.f - 13.f, mousePos.y + GuiParameters::slotSizeF / 2.f - 12.f});
    }

    hotbarGui.update();

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

    for (auto& slot : *hotbarGui.getGuiSlots()) {
        slot.setHovered(false);
        slot.update();
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

    for (auto& slot : *hotbarGui.getGuiSlots()) {
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

    hotbarGui.draw();

    if (carriedItemSprite) {
        window.draw(*(carriedItemSprite));

        if (carriedItem->getMaximalAmount() > 1)
            window.draw(amountOfCarriedItem);
    }
}