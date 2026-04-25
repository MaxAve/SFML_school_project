#pragma once
#include "core/Window.hpp"
#include "InventorySlot.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

// should only store inventory items
class Inventory {
    std::vector<std::vector<InventorySlot>> inventorySlots; // [x][y]
    sf::Vector2u inventorySize;
    std::string title;

public:
    Inventory(sf::Vector2u _inventorySize, const std::string& _title);

    // Item* popItem(sf::Vector2i);
    sf::Vector2u getInventorySize() const;

    std::vector<std::vector<InventorySlot>>* getInventorySlots();
    std::string getTitle() const;

    void setItem(sf::Vector2u slot, Item* item);
};