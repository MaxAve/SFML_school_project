#include "../../include/gui/Inventory.hpp"

const sf::Color Inventory::stdBackgroundColor(20, 20, 25, 150);
const sf::Color Inventory::stdForegroundColor(45, 50, 55, 230);
const float Inventory::stdOutlineThickness = 2;
const sf::Color Inventory::stdOutlineColor(100, 105, 115);

Inventory::Inventory(const sf::Vector2f& size, const sf::Vector2f& position) {
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

void Inventory::draw() {
    window.draw(background);
    window.draw(foreground);
}