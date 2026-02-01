#pragma once
#include "../Window.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "InventorySlot.hpp"
class Inventory {
    sf::RectangleShape background;
    sf::RectangleShape foreground;
    sf::Vector2f padding;

    std::vector<std::vector<InventorySlot>> inventorySlots; // [x][y]
    sf::Vector2u inventorySize;

    InventorySlot* hoveredSlot;

public:
    static const sf::Color stdBackgroundColor;
    static const sf::Color stdForegroundColor;
    static const float stdOutlineThickness;
    static const sf::Color stdOutlineColor;
    static unsigned slotSizeU;
    static float slotSizeF;

    Inventory(sf::Vector2u inventorySize, const sf::Vector2f& size, const sf::Vector2f& position = {0, 0});

    void resizeForeground(sf::Vector2f);

    void resizeBackground(sf::Vector2f);

    void setPosition(sf::Vector2f);

    sf::Vector2u getInventorySize() const;

    void setItem(sf::Vector2u slot, Item* item);

    void update();

    void draw();
};