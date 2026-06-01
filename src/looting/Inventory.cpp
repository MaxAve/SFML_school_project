#include "looting/Inventory.hpp"

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

sf::Vector2i Inventory::findEmptySlot() {
    for (size_t row = 0; row < inventorySize.x; ++row) {
        for (size_t line = 0; line < inventorySize.y; ++line) {
            if (!inventorySlots[row][line].getItem()) {
                return {static_cast<int>(row), static_cast<int>(line)};
            }
        }
    }

    return {-1, -1};
}

Item* Inventory::findItem(ItemType itemType) {
    for (auto& row : inventorySlots) {
        for (auto& slot : row) {
            if (slot.getItem()->getType() != itemType) {
                continue;
            }
            return slot.getItem();
        }
    }
    return nullptr;
}