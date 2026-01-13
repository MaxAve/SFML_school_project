#include "../include/Window.hpp"

sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "SFML Project");

sf::Vector2i Window::getMousePos()
{
	return sf::Mouse::getPosition(window); 
}

sf::Vector2i Window::getMousePosFromCenter()
{
	return sf::Vector2i(sf::Mouse::getPosition(window).x - WINDOW_WIDTH/2, sf::Mouse::getPosition(window).y - WINDOW_HEIGHT/2);
}
