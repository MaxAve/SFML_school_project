#pragma once
#include "core/Window.hpp"
#include "looting/Item.hpp"
#include <SFML/Graphics.hpp>

class DroppedItem {
    Item* item;
    sf::Sprite sprite;

    float totalTime;

public:
    static std::vector<DroppedItem*> pool;

    DroppedItem(Item*, sf::Vector2f pos);

    // TODO: DroppedItem* popItem()

    template <typename... Args>
    static DroppedItem* create(Args... args);

    void update();

    static void updateAll();

    void draw();

    static void drawAll();
};

template <typename... Args>
DroppedItem* DroppedItem::create(Args... args) {
    DroppedItem* ptr = new DroppedItem(args...);
    DroppedItem::pool.push_back(ptr);

    return ptr;
}