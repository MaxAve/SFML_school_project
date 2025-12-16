#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/player.h"
#include "../include/physics.h"
#include "../include/bullet.h"

int main() 
{
    Physics::init();
    sf::Clock deltaClock;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Project");

    Player player(window);
    
    Bullet bullet({400, 300}, 300, 0);

    sf::RectangleShape box(sf::Vector2f(50.f, 50.f));
    box.setFillColor(sf::Color::Red);
    box.setPosition(100.f, 240.f);

    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            player.sprite.move({-player.speed * Physics::deltaTime, 0});
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            player.sprite.move({player.speed * Physics::deltaTime, 0});
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            player.sprite.move({0, -player.speed * Physics::deltaTime});
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            player.sprite.move({0, player.speed * Physics::deltaTime});
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            bullet.sprite.setPosition(player.sprite.getPosition());
        }

        bullet.update();

        window.clear(sf::Color::Black);

        window.setView(player.view);

        player.draw(window);
        window.draw(box);
        bullet.draw(window);

        window.display();

        sf::Time dt = deltaClock.restart();
        Physics::deltaTime = dt.asSeconds();
    }

    return 0;
}
