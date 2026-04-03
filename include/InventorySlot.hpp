#pragma once
#include "Item.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

// not for gui
// to store items internally
class InventorySlot {
    Item* item;

public:
    InventorySlot();

    InventorySlot(Item* _item);

    Item* getItem();

    void setItem(Item* val);

    Item* popItem();
};
