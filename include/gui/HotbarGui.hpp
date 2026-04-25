#pragma once 
#include "looting/Hotbar.hpp"
#include "gui/InventorySlotGui.hpp"
#include "GuiParameters.hpp"

class HotbarGui {
    Hotbar* hotbar;
    std::vector<InventorySlotGui> inventorySlots;

    size_t markedSlot;
public:

    HotbarGui(Hotbar* hotbar, sf::Vector2f position = {0, 0});

    Hotbar* getHotbar();

    void setMarkedSlot(size_t idx);

    void setPosition(sf::Vector2f);

    void update();

    void draw();
};