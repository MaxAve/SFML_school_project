#include "../../include/gui/InventorySlotGui.hpp"
#include "../../include/gui/InventoryInterface.hpp"

const float InventorySlotGui::outlineThickness = InventoryInterface::stdOutlineThickness;
const sf::Color InventorySlotGui::outlineColor = InventoryInterface::stdOutlineColor;
const sf::Color InventorySlotGui::backgroundColor = InventoryInterface::stdForegroundColor;
const sf::Color InventorySlotGui::highlightColor = outlineColor;

InventorySlotGui::InventorySlotGui(InventorySlot* _inventorySlot) {
    hovered = false;
    inventorySlot = _inventorySlot;

    if (_inventorySlot) {
        setupItemSprite(_inventorySlot->getItem());
    }

    slotShape.setFillColor(backgroundColor);
    slotShape.setOutlineThickness(outlineThickness);
    slotShape.setOutlineColor(outlineColor);
}

InventorySlotGui::InventorySlotGui(sf::Vector2f position, float size) {
    slotShape.setSize({size, size});
    slotShape.setPosition(position);

    InventorySlotGui(nullptr);
}

bool InventorySlotGui::isHovered() const {
    return hovered;
}

void InventorySlotGui::setHovered(bool val) {
    hovered = val;

    if (hovered) {
        slotShape.setFillColor(highlightColor);
        return;
    }
    slotShape.setFillColor(backgroundColor);
}

void InventorySlotGui::setItem(Item* _item) {
    inventorySlot->setItem(_item);
    setupItemSprite(_item);
}

void InventorySlotGui::setupItemSprite(Item* item) {
    if (!item) {
        itemSprite.reset();
        return;
    }
    itemSprite.emplace(*(item->getTexture()));

    sf::Vector2f textureSize = (sf::Vector2f)item->getTexture()->getSize();
    float targetSize = slotShape.getSize().x;
    float scaleFactor = targetSize * 0.9f / std::max(textureSize.x, textureSize.y);
    itemSprite->setScale({scaleFactor, scaleFactor});
    itemSprite->setOrigin((*itemSprite).getLocalBounds().getCenter());
    itemSprite->setPosition(slotShape.getGlobalBounds().getCenter());
}

Item* InventorySlotGui::popItem() {
    Item* item = inventorySlot->popItem();

    itemSprite.reset();

    return item;
}

Item* InventorySlotGui::getItem() const {
    return inventorySlot->getItem();
}

const sf::Texture* InventorySlotGui::getSpriteTexture() const {
    return itemSprite ? &(*itemSprite).getTexture() : nullptr;
}

sf::FloatRect InventorySlotGui::getGlobalBounds() const {
    return slotShape.getGlobalBounds();
}

void InventorySlotGui::setPosition(sf::Vector2f _position) {
    slotShape.setPosition(_position);

    if (itemSprite) {
        itemSprite->setPosition(slotShape.getGlobalBounds().getCenter());
    }
}

sf::Vector2f InventorySlotGui::getPosition() const {
    return slotShape.getPosition();
}

InventorySlot* InventorySlotGui::getInventorySlot() const {
    return inventorySlot;
}

void InventorySlotGui::setInventorySlot(InventorySlot* _inventorySlot) {
    inventorySlot = _inventorySlot;

    if (_inventorySlot) {
        setupItemSprite(_inventorySlot->getItem());
    } else {
        itemSprite.reset();
    }
}

void InventorySlotGui::setSize(float _size) {
    slotShape.setSize({_size, _size});
}

void InventorySlotGui::draw() const {
    window.draw(slotShape);

    if (itemSprite) {
        window.draw(*itemSprite);
    }
}