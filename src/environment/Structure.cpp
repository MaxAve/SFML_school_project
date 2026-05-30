#include "environment/Structure.hpp"

std::vector<Structure*> Structure::pool;

Structure::Structure(size_t type_, sf::Vector2f position_) : type{type_}, position{position_}, rect({250, 500}) 
{
    rect.setFillColor(sf::Color::Green);
    rect.setPosition(position_);
}

sf::Vector2f Structure::getPosition() const {
    return position;
}

size_t Structure::getType() const {
    return type;
}

void Structure::draw() {
    window.draw(rect);
}

void Structure::drawAll() {
    for (auto& structure : pool) {
        structure->draw();
    }
}