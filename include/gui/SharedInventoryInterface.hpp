#pragma once
#include "gui/InventorySlotGui.hpp"
#include "looting/Inventory.hpp"
#include "gui/GuiParameters.hpp"
#include "gui/HotbarGui.hpp"

class SharedInventoryInterface {
    Inventory* mainInventory;
    Hotbar* hotbar;
    Inventory* otherInventory;

    std::vector<std::vector<InventorySlotGui>> mainInventorySlots;
    sf::Text mainTitle;
    HotbarGui hotbarGui;

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
    SharedInventoryInterface(sf::Vector2f size, sf::Vector2f position = {0, 0}, Inventory* mainInventory = nullptr, Hotbar* hotbar = 0, Inventory* otherInventory = nullptr);

    void setCarriedItemSprite(sf::Texture*);

    void handleLMB(sf::Vector2f mousePos);

    void handleRMB(sf::Vector2f mousePos);

    void resizeForeground(sf::Vector2f);

    void resizeBackground(sf::Vector2f);

    void setMainInventory(Inventory* inventory);

    void setHotbar(Hotbar* hotbar);

    void setOtherInventory(Inventory* inventory);

    Inventory* getOtherInventory();

    Inventory* getMainInventory();

    Hotbar* getHotbar();

    InventorySlotGui* findHoveredSlot();

    void setPosition(sf::Vector2f);

    void update();

    void draw();
};