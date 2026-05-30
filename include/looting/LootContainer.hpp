#pragma once
#include "InventorySlot.hpp"
#include "entities/Player.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class LootContainer {
    static float promptDistanceFromContainer;

    float range;
    sf::RectangleShape box;
    bool playerInRange = false;
    sf::Sprite prompt;
    float totalTime = 0;
    sf::Vector2f promptDefPos;

    Inventory inventory;

public:
    static std::vector<LootContainer*> pool;

    size_t type;

    LootContainer(size_t _type, sf::Vector2f _pos, sf::Vector2f _size, float _range);

    // ! returns distance as square
    double getDistanceToSq(const sf::Vector2f);

    double getDistanceTo(const sf::Vector2f);

    std::vector<std::vector<InventorySlot>>* getInventorySlots();

    // ! arg is distance as square
    bool inRangeSq(double);

    // in addition to constructor, adds the created obj to the pool
    template <typename... Args>
    static LootContainer* create(Args&&... args);

    bool isPlayerInRange() const;

    const sf::Vector2f getPosition() const;

    Inventory* getInventory();

    void update(const Player& player);

    void updateAll(const Player& player);

    void draw();

    static void drawAll();
};

// template function together with header
template <typename... Args>
LootContainer* LootContainer::create(Args&&... args) {
    LootContainer* ptr = new LootContainer(args...);
    LootContainer::pool.push_back(ptr);

    return ptr;
}