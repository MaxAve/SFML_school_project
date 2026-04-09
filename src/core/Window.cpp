#include "core/Window.hpp"
#include "entities/Player.hpp"

sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "SFML Project");
sf::View defaultView(window.getDefaultView());

sf::Vector2i Window::getMousePos(const sf::View& view) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return sf::Vector2i(window.mapPixelToCoords({mousePos.x, mousePos.y}, view));
}

sf::Vector2i Window::getMousePosFromCenter(const sf::View& view) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return sf::Vector2i(window.mapPixelToCoords({mousePos.x - (int)window.getSize().x / 2, mousePos.y - (int)window.getSize().y / 2}, view));
}
