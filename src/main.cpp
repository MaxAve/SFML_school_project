#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/Player.h"
#include "../include/Physics.h"
#include "../include/Bullet.h"
#include "../include/window.h"
#include "../include/TileMap.hpp"
#include "../include/Zombie.h"

int main() 
{
    srand(time(NULL));
    Physics::init();
    sf::Clock deltaClock;

    TileMap tileMap(20,20, 50.f);

    Player player(window);
	float fireRate = 10.0f;
	float timeSinceLastShot = 0.0f;

    Zombie testZombie1({0, 0}, &player);
    Zombie testZombie2({100, 0}, &player);
    Zombie testZombie3({200, 0}, &player);
    Zombie testZombie4({300, 0}, &player);
    Zombie testZombie5({400, 0}, &player);

    while (window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
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
				
				//std::cout << Bullet::pool.size() << " - " << Bullet::pool[0]->distanceTraveled << "\n";
			}
        }

		timeSinceLastShot += Physics::deltaTime;

		Bullet::updateAll();
		player.update();
        Zombie::updateAll();

        window.clear(sf::Color::Black);

        window.setView(player.view);
        tileMap.drawAll(window);
		Bullet::drawAll(window);
        Zombie::drawAll(window);

        player.draw(window);

        window.display();

        sf::Time dt = deltaClock.restart();
        Physics::deltaTime = dt.asSeconds();
    }

    return 0;
}
