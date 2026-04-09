#include "../../include/looting/Inventory.hpp"

Inventory::Inventory(sf::Vector2u _inventorySize, const std::string& _title) : title{_title} {
    this->inventorySize = _inventorySize;
    inventorySlots.resize(inventorySize.x);
    for (size_t x = 0; x < inventorySlots.size(); x++) {
        inventorySlots[x].resize(inventorySize.y);
    }
}

sf::Vector2u Inventory::getInventorySize() const {
    return inventorySize;
}

std::string Inventory::getTitle() const {
    return title;
}

std::vector<std::vector<InventorySlot>>* Inventory::getInventorySlots() {
    return &inventorySlots;
}

void Inventory::setItem(sf::Vector2u slot, Item* item) {
    inventorySlots[slot.x][slot.y].setItem(item);
}
