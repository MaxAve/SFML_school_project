#include "gui/HotbarGui.hpp"

unsigned HotbarGui::slotSizeU = 95u;
float HotbarGui::slotSizeF = static_cast<float>(slotSizeU);

HotbarGui::HotbarGui(Hotbar* _hotbar, sf::Vector2f _position) : hotbar(_hotbar) {
    size_t size = hotbar->getSize();
    auto originalSlots = hotbar->getInventorySlots();

    inventorySlots.resize(size);
    for (size_t i = 0; i < size; i++) {
        inventorySlots[i].setInventorySlot(&(*originalSlots)[i]);
        inventorySlots[i].setSize(slotSizeF);
    }

    setPosition(_position);
}

Hotbar* HotbarGui::getHotbar() {
    return hotbar;
}

void HotbarGui::setMarkedSlot(size_t idx) {
    if (idx > hotbar->getSize()) {
        return;
    }
    
    for (auto& slot : inventorySlots) {
        slot.setHovered(false);
    }

    inventorySlots[idx].setHovered(true);
}

// start with left upper corner of all slots
void HotbarGui::setPosition(sf::Vector2f pos) {
    float idx = 0;
    for (auto& slot : inventorySlots) {
        slot.setPosition(pos + sf::Vector2f{slotSizeF, 0} * idx);
        idx++;
    }
}

void HotbarGui::update() {
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