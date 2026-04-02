#include "../../include/gui/SharedInventoryInterface.hpp"
#include "../../include/gui/InventoryInterface.hpp"
#include "../../include/fonts.hpp"

const sf::Color SharedInventoryInterface::stdBackgroundColor = InventoryInterface::stdBackgroundColor;
const sf::Color SharedInventoryInterface::stdForegroundColor = InventoryInterface::stdForegroundColor;
const float SharedInventoryInterface::stdOutlineThickness = InventoryInterface::stdOutlineThickness;
const sf::Color SharedInventoryInterface::stdOutlineColor = InventoryInterface::stdOutlineColor;
unsigned SharedInventoryInterface::slotSizeU = InventoryInterface::slotSizeU;
float SharedInventoryInterface::slotSizeF = InventoryInterface::slotSizeF;

SharedInventoryInterface::SharedInventoryInterface(sf::Vector2f _size, sf::Vector2f _position, Inventory* _mainInventory, Inventory* _otherInventory) : mainTitle(Fonts::pixel), otherTitle(Fonts::pixel), mainInventory{_mainInventory}, otherInventory{_otherInventory} {
    carriedItem = nullptr;
    carriedItemSprite.reset();

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

    // TODO: padding, inventorySlots
}

// TODO: all empty functions

void SharedInventoryInterface::handleMousePress(sf::Vector2f mousePos) {

}

void SharedInventoryInterface::resizeForeground(sf::Vector2f) {

}

void SharedInventoryInterface::resizeBackground(sf::Vector2f) {

}

void SharedInventoryInterface::setMainInventory(Inventory* inventory) {

}

void SharedInventoryInterface::setOtherInventory(Inventory* inventory) {

}

sf::Vector2i SharedInventoryInterface::getSlotByScreenCoord(sf::Vector2f) {
    return {0, 0};
}

void SharedInventoryInterface::setPosition(sf::Vector2f) {

}

void SharedInventoryInterface::update() {
    
}

void SharedInventoryInterface::draw() {
    window.draw(background);
    window.draw(foreground);
}