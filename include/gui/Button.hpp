#pragma once
#include <functional>
#include <string>
#include <SFML/Graphics.hpp>

class Button {
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> action; // The callback function

    bool hovered = false;

    static sf::Color buttonColor;
    static sf::Color highlightColor;
    static sf::Color outlineColor;
public:
    Button(sf::Vector2f position, sf::Vector2f size, const std::string& text , std::function<void()> lambda);

    sf::Vector2f getPosition() const;

    sf::Vector2f getSize() const;

    bool isHovered() const;

    void setHovered(bool val);

    void setPosition(sf::Vector2f);

    void run();

    void update(sf::Vector2f mousePos);

    void draw();
};