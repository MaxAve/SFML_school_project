#pragma once
#include "core/Window.hpp"
#include "looting/Item.hpp"
#include <SFML/Graphics.hpp>
#include "entities/Player.hpp"

class DroppedItem {
    Item* item;
    sf::Sprite sprite;
    bool readyForPickup = false;

    float totalTime;
    float pickupCountdown;

public:
    static std::vector<DroppedItem*> pool;

    DroppedItem(Item*, sf::Vector2f pos);

    Item* popItem();

    Item* getItem();

    bool isPlayerInRange(sf::Vector2f playerPos) const;

    bool isReadyForPickup() const;

    template <typename... Args>
    static DroppedItem* create(Args... args);

    void update(Player& player);

    static void updateAll(Player& player);

    void draw();

    static void drawAll();
};

template <typename... Args>
DroppedItem* DroppedItem::create(Args... args) {
    DroppedItem* ptr = new DroppedItem(args...);
    DroppedItem::pool.push_back(ptr);

    return ptr;
}