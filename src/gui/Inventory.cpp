#include "../../include/gui/Inventory.hpp"

const sf::Color Inventory::stdBackgroundColor(20, 20, 25, 150);
const sf::Color Inventory::stdForegroundColor(45, 50, 55, 230);
const float Inventory::stdOutlineThickness = 2;
const sf::Color Inventory::stdOutlineColor(100, 105, 115);
unsigned Inventory::slotSizeU = 95u;
float Inventory::slotSizeF = static_cast<float>(slotSizeU);

Inventory::Inventory(sf::Vector2u inventorySize, const sf::Vector2f& size, const sf::Vector2f& position) {
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
    sf::Vector2f topLeftCorner = foreground.getPosition() - foreground.getSize() / 2.f;
    sf::Vector2f padding = (size - slotSizeF * (sf::Vector2f)inventorySize) / 2.f;

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

void Inventory::setPosition(sf::Vector2f newPosition) {
    foreground.setPosition(newPosition);
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

    for (auto& line : inventorySlots) {
        for (auto& slot : line) {
            if (slot.getGlobalBounds().contains(mousePos)) {
                    slot.setHovered(true);
                }
            else {
                slot.setHovered(false);
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
}