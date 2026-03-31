#pragma once
#include "../Inventory.hpp"

class SharedInventoryInterface {
    Inventory* mainInventory;
    Inventory* otherInventory;

    std::vector<std::vector<InventorySlot>>* mainInventorySlots;
    sf::Text mainTitle;
    std::vector<std::vector<InventorySlot>>* otherInventorySlots;
    sf::Text otherTitle;

    sf::RectangleShape background;
    sf::RectangleShape foreground;
    sf::Vector2f padding;

    // For Drag & Drop
    Item* carriedItem;
    std::optional<sf::Sprite> carriedItemSprite;

    InventorySlot* hoveredSlot;

public:
    static const sf::Color stdBackgroundColor;
    static const sf::Color stdForegroundColor;
    static const float stdOutlineThickness;
    static const sf::Color stdOutlineColor;
    static unsigned slotSizeU;
    static float slotSizeF;

    SharedInventoryInterface();

};