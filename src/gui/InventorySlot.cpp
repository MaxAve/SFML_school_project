#include "../../include/gui/InventorySlot.hpp"
#include "../../include/Window.hpp"
#include "../../include/gui/Inventory.hpp"

const float InventorySlot::outlineThickness = Inventory::stdOutlineThickness;
const sf::Color InventorySlot::outlineColor = Inventory::stdOutlineColor;
const sf::Color InventorySlot::backgroundColor = Inventory::stdForegroundColor;
const sf::Color InventorySlot::highlightColor = outlineColor;

InventorySlot::InventorySlot() {
    item = nullptr;
    hovered = false;

    slotShape.setFillColor(backgroundColor);
    slotShape.setOutlineThickness(outlineThickness);
    slotShape.setOutlineColor(outlineColor);
}

InventorySlot::InventorySlot(sf::Vector2f position, float size) : InventorySlot() {
    slotShape.setSize({size, size});
    slotShape.setPosition(position);
}

bool InventorySlot::isHovered() const {
    return hovered;
}

void InventorySlot::setHovered(bool val) {
    hovered = val;

    if (hovered) {
        slotShape.setFillColor(highlightColor);
        return;
    }
    slotShape.setFillColor(backgroundColor);
}

void InventorySlot::setItem(Item* _item) {
    item = _item;

    if (!_item) {
        itemSprite.reset();
        return;
    }
    itemSprite.emplace(*(_item->getTexture()));

    sf::Vector2f textureSize = (sf::Vector2f) _item->getTexture()->getSize();
    float targetSize = slotShape.getSize().x;
    float scaleFactor = targetSize * 0.9f / std::max( textureSize.x, textureSize.y );
    itemSprite->setScale({ scaleFactor, scaleFactor });
    itemSprite->setOrigin((*itemSprite).getLocalBounds().getCenter());
    itemSprite->setPosition(slotShape.getGlobalBounds().getCenter());
}

Item* InventorySlot::popItem() {
    Item* _item = item;
    
    itemSprite.reset();
    item = nullptr;

    return _item;
}

Item* InventorySlot::getItem() const {
    return item;
}

sf::FloatRect InventorySlot::getGlobalBounds() const {
    return slotShape.getGlobalBounds();
}

void InventorySlot::setPosition(sf::Vector2f _position) {
    slotShape.setPosition(_position);

    if (itemSprite) {
        itemSprite->setPosition(slotShape.getGlobalBounds().getCenter());
    }
}

sf::Vector2f InventorySlot::getPosition() const {
    return slotShape.getPosition();
}

void InventorySlot::setSize(float _size) {
    slotShape.setSize({_size, _size});
}

void InventorySlot::draw() {
    window.draw(slotShape);

    if (itemSprite) {
        window.draw(*itemSprite);
    }
}