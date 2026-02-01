#pragma once
#include <SFML/Graphics.hpp>

class InventorySlot {
    static const float outlineThickness;
    static const sf::Color outlineColor;
    static const sf::Color backgroundColor;
    static const sf::Color highlightColor;

    sf::RectangleShape slotShape; // includes position
    bool hovered;

public:
    InventorySlot();

    InventorySlot(sf::Vector2f position, float size);

    bool isHovered() const;

    void setHovered(bool val);

    sf::FloatRect getGlobalBounds() const;

    void setPosition(sf::Vector2f _position);

    sf::Vector2f getPosition() const;

    void setSize(float _size);

    void draw();
};

