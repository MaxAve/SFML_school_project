#include "looting/Hotbar.hpp"



Hotbar::Hotbar(size_t _size) : size{_size} {
    inventorySlots.resize(_size);
}

std::vector<InventorySlot>* Hotbar::getInventorySlots() {
    return &inventorySlots;
}

size_t Hotbar::getSize() const {
    return size;
}

void Hotbar::setItem(size_t index, Item* item) {
    if (index > size) {
        assert(false);  // set out of index
        return;
    }
    inventorySlots[index].setItem(item);
}

// Item* Hotbar::popItem(size_t index) {
    
// }