#include "../include/Bullet.hpp"
#include "../include/Physics.hpp"
#include "../include/Player.hpp"
#include "../include/PlayerHealthBar.hpp"
#include "../include/TileMap.hpp"
#include "../include/Window.hpp"
#include "../include/Zombie.hpp"
#include "../include/fonts.hpp"
#include "../include/BulletMeter.hpp"
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

    TileMap tileMap(20, 20, 80.f);

    Player player(window);
    PlayerHealthBar playerHealthBar({200, 20}, 100);
    bool inventoryToggled;

    // ! TEST
    Item grassBfr(false, false, false, 0, 0, Textures::get(Textures::TextureType::Grass));
    player.inventory.setItem({1, 1}, &grassBfr);
    // !

    BulletMeter bulletMeter(sf::Vector2f(5, 70), 30);

    float fireRate = 10.0f;
    float timeSinceLastShot = 0.0f;

    for (int i = 0; i < 20; i++) {
        new Zombie({(float)(rand() % 800), 0}, &player);
    }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::Resized>()) {
                sf::Vector2f newSize = {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)};

                defaultView.setSize({newSize.x, newSize.y});
                defaultView.setCenter({newSize.x / 2, newSize.y / 2});
                player.view.setSize({newSize.x, newSize.y});
                player.inventory.resizeBackground(newSize);
                // player.inventory.resizeForeground({ newSize.x * 0.75f, newSize.y * 0.75f });
                player.inventory.setPosition({newSize.x / 2, newSize.y / 2});
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
                    window.close();
                if (keyPressed->scancode == sf::Keyboard::Scan::I) {
                    inventoryToggled = !inventoryToggled;
                }
            }
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(Window::getMousePos());
                    player.inventory.handleMousePress(mousePos);
                }
            }
        }

        if (!inventoryToggled) {
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
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (timeSinceLastShot >= (1.0f / fireRate)) {
                    sf::Vector2i mousePos = Window::getMousePos();
                    float angle = std::atan2(mousePos.y - defaultView.getSize().y / 2, mousePos.x - defaultView.getSize().x / 2);
                    Bullet* b = new Bullet({player.sprite.getPosition().x + player.sprite.getSize().x / 2, player.sprite.getPosition().y + player.sprite.getSize().y / 2}, 2000, angle);
                    timeSinceLastShot = 0.0f;
                    
                    if(bulletMeter.currentBullets > 0)
                    {
                        bulletMeter.setCurrentBullets(bulletMeter.currentBullets - 1);
                    }
                }
            }
        }
        timeSinceLastShot += Physics::deltaTime;

        Bullet::updateAll();
        player.update();
        Zombie::updateAll();
        Particle::updateAll();
        if (inventoryToggled) {
            player.inventory.update();
        }

        window.clear(sf::Color::Black);

        // draw Camera (View)
        window.setView(player.view);

        tileMap.draw(window);
        Particle::drawOnlyNonActive(window);
        Bullet::drawAll(window);
        Zombie::drawAll(window);
        player.draw(window);
        Particle::drawOnlyActive(window);

        // draw UI
        window.setView(defaultView);
        playerHealthBar.draw(window);
        bulletMeter.draw(window);

        if (inventoryToggled) {
            player.inventory.draw();
        }

        window.display();

        sf::Time dt = deltaClock.restart();
        Physics::deltaTime = dt.asSeconds();
    }

    return 0;
}
