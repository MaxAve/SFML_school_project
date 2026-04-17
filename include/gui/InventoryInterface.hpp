#pragma once
#include "looting/Inventory.hpp"
#include "core/Window.hpp"
#include "gui/InventorySlotGui.hpp"
#include "resources/Fonts.hpp"
#include "gui/ItemLabel.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// manages the graphical implementation of the inventory
class InventoryInterface {
    Inventory* inventory;
    std::vector<std::vector<InventorySlotGui>> inventorySlots;
    sf::Text title;

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

    InventoryInterface(Inventory* inventory, sf::Vector2f size, sf::Vector2f position = {0, 0});

    void handleMousePress(sf::Vector2f mousePos);

    void resizeForeground(sf::Vector2f);

    void resizeBackground(sf::Vector2f);

    sf::Vector2i getSlotByScreenCoord(sf::Vector2f);

    void setPosition(sf::Vector2f);

    void update();

    void draw();
};