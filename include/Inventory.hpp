#pragma once
#include "Window.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "gui/InventorySlot.hpp"

// should only store inventory items
class Inventory {
    std::vector<std::vector<InventorySlot>> inventorySlots; // [x][y]
    sf::Vector2u inventorySize;

public:
    Inventory(sf::Vector2u inventorySize);
    
    Item* popItem(sf::Vector2i);

    sf::Vector2u getInventorySize() const;

    std::vector<std::vector<InventorySlot>>* getInventorySlots();

    void setItem(sf::Vector2u slot, Item* item);
};