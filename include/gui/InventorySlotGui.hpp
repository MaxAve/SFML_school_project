#pragma once
#include "../looting/InventorySlot.hpp"

// supposed to show the inventory slot in the gui
class InventorySlotGui {
    static const float outlineThickness;
    static const sf::Color outlineColor;
    static const sf::Color backgroundColor;
    static const sf::Color highlightColor;

    InventorySlot* inventorySlot;

    sf::RectangleShape slotShape; // includes position
    bool hovered;
    std::optional<sf::Sprite> itemSprite;

public:

    InventorySlotGui(InventorySlot* _inventorySlot = nullptr);

    InventorySlotGui(sf::Vector2f position, float size);

    bool isHovered() const;

    void setHovered(bool val);

    void setItem(Item* _item);

    void setupItemSprite(Item* item);

    Item* popItem();

    Item* getItem() const;

    const sf::Texture* getSpriteTexture() const;

    sf::FloatRect getGlobalBounds() const;

    void setPosition(sf::Vector2f _position);

    sf::Vector2f getPosition() const;

    InventorySlot* getInventorySlot() const;

    void setInventorySlot(InventorySlot* _inventorySlot);

    void setSize(float _size);

    void draw() const;
};