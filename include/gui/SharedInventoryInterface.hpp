#pragma once
#include "gui/InventorySlotGui.hpp"
#include "looting/Inventory.hpp"

class SharedInventoryInterface {
    Inventory* mainInventory;
    Inventory* otherInventory;

    std::vector<std::vector<InventorySlotGui>> mainInventorySlots;
    sf::Text mainTitle;
    std::vector<std::vector<InventorySlotGui>> otherInventorySlots;
    sf::Text otherTitle;

    sf::RectangleShape background;
    sf::RectangleShape foreground;
    sf::Vector2f paddingU; // for upper sector
    sf::Vector2f paddingL; // for lower sector

    // For Drag & Drop
    Item* carriedItem;
    std::optional<sf::Sprite> carriedItemSprite;
    sf::Text amountOfCarriedItem;

    InventorySlot* hoveredSlot = nullptr;

public:
    static const sf::Color backgroundColor;
    static const sf::Color foregroundColor;
    static const float outlineThickness;
    static const sf::Color outlineColor;
    static unsigned slotSizeU;
    static float slotSizeF;

    SharedInventoryInterface(sf::Vector2f size, sf::Vector2f position = {0, 0}, Inventory* mainInventory = nullptr, Inventory* otherInventory = nullptr);

    void setCarriedItemSprite(sf::Texture*);

    void handleLMB(sf::Vector2f mousePos);

    void handleRMB(sf::Vector2f mousePos);

    void resizeForeground(sf::Vector2f);

    void resizeBackground(sf::Vector2f);

    void setMainInventory(Inventory* inventory);

    Inventory* getOtherInventory();

    Inventory* getMainInventory();

    void setOtherInventory(Inventory* inventory);

    InventorySlotGui* findHoveredSlot();

    void setPosition(sf::Vector2f);

    void update();

    void draw();
};