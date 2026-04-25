#pragma once
#include "InventorySlot.hpp"

class Hotbar {
    std::vector<InventorySlot> inventorySlots;
    size_t size;

public:
    Hotbar(size_t size);

    std::vector<InventorySlot>* getInventorySlots();

    // Item* popItem(size_t);

    size_t getSize() const;

    void setItem(size_t index, Item* item);
};