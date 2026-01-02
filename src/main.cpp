#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/player.h"
#include "../include/physics.h"
#include "../include/bullet.h"
#include "../include/window.h"

int main() 
{
    Physics::init();
    sf::Clock deltaClock;

    Player player(window);
	float fireRate = 10.0f;
	float timeSinceLastShot = 0.0f;
    
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
			if(timeSinceLastShot >= (1.0f / fireRate))
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				float angle = std::atan2(mousePos.y - WINDOW_HEIGHT/2, mousePos.x - WINDOW_WIDTH/2);
				Bullet* b = new Bullet({player.sprite.getPosition().x + player.sprite.getSize().x/2 - 10, player.sprite.getPosition().y + player.sprite.getSize().y/2 - 10}, 2000, angle);
				timeSinceLastShot = 0.0f;
				
				std::cout << Bullet::pool.size() << " - " << Bullet::pool[0]->distanceTraveled << "\n";
			}
        }

		timeSinceLastShot += Physics::deltaTime;

		Bullet::updateAll();
		player.update();

        window.clear(sf::Color::Black);

        window.setView(player.view);

		Bullet::drawAll(window);
        player.draw(window);
        window.draw(box);

        window.display();

        sf::Time dt = deltaClock.restart();
        Physics::deltaTime = dt.asSeconds();
    }

    return 0;
}
