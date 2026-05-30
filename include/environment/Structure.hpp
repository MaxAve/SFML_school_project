#pragma once
#include <SFML/Graphics.hpp>
#include "core/Window.hpp"
#include <vector>

class Structure {
    sf::Vector2f position;
    size_t type; // TODO change later to enum

    sf::RectangleShape rect; // ! temporary
                             // TODO: replace with sprite

public:
    static std::vector<Structure*> pool;

    Structure(size_t type, sf::Vector2f position);

    // allocates Structure and adds it to the pool
    template <typename... Args>
    static Structure* create(Args... args);

    sf::Vector2f getPosition() const;

    size_t getType() const;

    void draw();

    static void drawAll();
};

template <typename... Args>
Structure* Structure::create(Args... args) {
    Structure* ptr = new Structure(args...);
    Structure::pool.push_back(ptr);

    return ptr;
}