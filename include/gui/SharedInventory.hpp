#pragma once
#include "Inventory.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

class SharedInventory {
    Inventory* first;
    Inventory* second;

    Item* carriedItem;
    std::optional<sf::Sprite> carriedItemSprite;

public:
    SharedInventory();

    SharedInventory(Inventory* _first, Inventory* _second);

    void handleMousePress(sf::Vector2f mousePos);

    void setFirst(Inventory* _first);

    void setSecond(Inventory* _second);

    Inventory* getFirst() const;

    Inventory* getSecond() const;

    void update();

    void draw();
};