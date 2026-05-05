#include "gui/HotbarGui.hpp"

#include <iostream>
#define LOG(msg) std::cout << msg << std::endl;

HotbarGui::HotbarGui(Hotbar* _hotbar, sf::Vector2f _position) {
    setHotbar(_hotbar);
    setPosition(_position);
}

Hotbar* HotbarGui::getHotbar() {
    return hotbar;
}

std::vector<InventorySlotGui>* HotbarGui::getGuiSlots() {
    return &inventorySlots;
}

void HotbarGui::setHotbar(Hotbar* _hotbar) {
    hotbar = _hotbar;
    
    if (!hotbar) {
        return;
    }

    size_t size = hotbar->getSize();
    auto originalSlots = hotbar->getInventorySlots();

    inventorySlots.resize(size);
    for (size_t i = 0; i < size; i++) {
        inventorySlots[i].setInventorySlot(&(*originalSlots)[i]);
        inventorySlots[i].setSize(GuiParameters::slotSizeF);
    }
}

void HotbarGui::setMarkedSlot(size_t idx) {
    if (idx >= hotbar->getSize()) {
        return;
    }
    
    for (auto& slot : inventorySlots) {
        slot.setHovered(false);
    }

    inventorySlots[idx].setHovered(true);
    this->markedSlot = idx;
}

// start with left upper corner of all slots
void HotbarGui::setPosition(sf::Vector2f pos) {
    float idx = 0;
    for (auto& slot : inventorySlots) {
        slot.setPosition(pos + sf::Vector2f{GuiParameters::slotSizeF, 0} * idx);
        idx++;
    }
}

Item *HotbarGui::getItem(size_t slot)
{
    if(slot < 0 || slot > 4)
        return nullptr;
    return this->inventorySlots[slot].getInventorySlot()->getItem();
}

Item *HotbarGui::getSelectedItem()
{
    return this->getItem(this->markedSlot);
}

void HotbarGui::update() {
    if (!hotbar) {
        return;
    }

    // set inventorySlotsGui according to original slots
    size_t size = hotbar->getSize();
    auto originalSlots = hotbar->getInventorySlots();
    for (size_t i = 0; i < size; i++) {
        inventorySlots[i].setItem((*originalSlots)[i].getItem());
        inventorySlots[i].update();
    }
}

void HotbarGui::draw() {
    for (const auto& slot : inventorySlots) {
        slot.draw();
    }
}