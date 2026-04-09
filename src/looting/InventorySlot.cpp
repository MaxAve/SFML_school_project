#include "looting/InventorySlot.hpp"
#include "core/Window.hpp"
#include "gui/InventoryInterface.hpp"

InventorySlot::InventorySlot() {
    item = nullptr;
}

void InventorySlot::setItem(Item* _item) {
    item = _item;
}

Item* InventorySlot::popItem() {
    Item* _item = item;
    item = nullptr;

    return _item;
}

Item* InventorySlot::getItem() {
    return item;
}
