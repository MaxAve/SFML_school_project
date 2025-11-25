#include <SFML/Graphics.hpp>

int main() {
    // Create window (800x600)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rectangle Example");

    // Create a rectangle
    sf::RectangleShape rectangle(sf::Vector2f(200.f, 120.f));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setPosition(300.f, 240.f); // Center-ish

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.display();
    }

    return 0;
}
