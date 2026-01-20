#include "../include/Bullet.hpp"
#include "../include/Physics.hpp"
#include "../include/Player.hpp"
#include "../include/PlayerHealthBar.hpp"
#include "../include/TileMap.hpp"
#include "../include/Window.hpp"
#include "../include/Zombie.hpp"
#include "../include/fonts.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    srand(time(NULL));
    Physics::init();
    Textures::initTextures();
    Fonts::initFonts();
    sf::Clock deltaClock;

    window.setFramerateLimit(60); // to avoid pc flying into space
    window.setView(defaultView);

    TileMap tileMap(20, 20, 150.f);

    Player player(window);
    PlayerHealthBar playerHealthBar({200, 20}, 100);
    float fireRate = 10.0f;
    float timeSinceLastShot = 0.0f;

    sf::RectangleShape testShape({5.f,5.f});


    for (int i = 0; i < 20; i++) {
        new Zombie({(float)(rand() % 800), 0}, &player);
    }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::Resized>()) {
                defaultView.setSize({(float)window.getSize().x, (float)window.getSize().y});
                defaultView.setCenter({(float) window.getSize().x / 2, (float) window.getSize().y / 2});
                player.view.setSize({(float)window.getSize().x, (float)window.getSize().y});
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
                    window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            player.sprite.move({-player.speed * Physics::deltaTime, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            player.sprite.move({player.speed * Physics::deltaTime, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            player.sprite.move({0, -player.speed * Physics::deltaTime});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            player.sprite.move({0, player.speed * Physics::deltaTime});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            if (timeSinceLastShot >= (1.0f / fireRate)) {
                sf::Vector2i mousePos = Window::getMousePos();
                float angle = std::atan2(mousePos.y - WINDOW_HEIGHT / 2, mousePos.x - WINDOW_WIDTH / 2);
                Bullet* b = new Bullet({player.sprite.getPosition().x + player.sprite.getSize().x / 2, player.sprite.getPosition().y + player.sprite.getSize().y / 2}, 2000, angle);
                timeSinceLastShot = 0.0f;

                // std::cout << Bullet::pool.size() << " - " << Bullet::pool[0]->distanceTraveled << "\n";
            }
        }

        timeSinceLastShot += Physics::deltaTime;

        Bullet::updateAll();
        player.update();
        Zombie::updateAll();
        testShape.setPosition(defaultView.getCenter());

        window.clear(sf::Color::Black);

        // draw Camera (View)
        window.setView(player.view);

        tileMap.draw(window);
        Bullet::drawAll(window);
        Zombie::drawAll(window);
        player.draw(window);

        // draw UI
        window.setView(defaultView);
        playerHealthBar.draw(window);
        window.draw(testShape);

        window.display();

        sf::Time dt = deltaClock.restart();
        Physics::deltaTime = dt.asSeconds();
    }

    return 0;
}
