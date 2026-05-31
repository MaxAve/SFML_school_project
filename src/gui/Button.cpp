#include "gui/Button.hpp"
#include "core/Window.hpp"
#include "resources/Fonts.hpp"

sf::Color Button::buttonColor = sf::Color::Red;
sf::Color Button::highlightColor = sf::Color::Yellow;
sf::Color Button::outlineColor = sf::Color::Black;

Button::Button(sf::Vector2f position, sf::Vector2f size, const std::string& text_, std::function<void()> lambda)
    : text(Fonts::pixel, text_, 30U), action{lambda} {
    shape.setFillColor(buttonColor);
    shape.setSize(size);
    shape.setOrigin(shape.getLocalBounds().getCenter());
    shape.setPosition(position);

    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition(position);
    text.setFillColor(sf::Color::White);
}

sf::Vector2f Button::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Button::getSize() const {
    return shape.getSize();
}

bool Button::isHovered() const {
    return hovered;
}

void Button::setHovered(bool val) { hovered = val; }

void Button::setPosition(sf::Vector2f pos) {
    shape.setPosition(pos);
    text.setPosition(pos);
}

void Button::run() {
    if (action)
        action();
}

void Button::update(sf::Vector2f mousePos) {
    if (shape.getGlobalBounds().contains(mousePos)) {
        shape.setFillColor(highlightColor);
        hovered = true;
    } else {
        shape.setFillColor(buttonColor);
        hovered = false;
    }
}

void Button::draw() {
    window.draw(shape);
    window.draw(text);
}
