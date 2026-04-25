#pragma once 
#include "looting/Hotbar.hpp"
#include "gui/InventorySlotGui.hpp"

class HotbarGui {
    Hotbar* hotbar;
    std::vector<InventorySlotGui> inventorySlots;

    size_t markedSlot;
public:
    static const float stdOutlineThickness;
    static const sf::Color stdOutlineColor;
    static unsigned slotSizeU;
    static float slotSizeF;

    HotbarGui(Hotbar* hotbar, sf::Vector2f size, sf::Vector2f position = {0, 0});

    Hotbar* getHotbar();

    void setMarkedSlot();

    void setPosition(sf::Vector2f);

    void update();

    void draw();
};