#include "../include/Bullet.hpp"
#include "../include/Physics.hpp"
#include "../include/Player.hpp"
#include "../include/gui/PlayerHealthBar.hpp"
#include "../include/TileMap.hpp"
#include "../include/Window.hpp"
#include "../include/Zombie.hpp"
#include "../include/fonts.hpp"
#include "../include/gui/BulletMeter.hpp"
#include "../include/Door.hpp"
#include "../include/LootContainer.hpp"
#include "../include/fx/DamageIndicatorText.hpp"
#include "../include/gui/InventoryInterface.hpp"
#include "../include/Inventory.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#define LOG(message) std::cout << message << std::endl

float randomFloat(float min, float max) {
    static std::random_device rd;      // seed
    static std::mt19937 gen(rd());     // Mersenne Twister RNG
    std::uniform_real_distribution<float> dist(min, max);

    return dist(gen);
}

int main() {
    LOG("main()");

    LOG("initializing time, physics, textures, fonts");
    srand(time(NULL));
    Physics::init();
    Textures::initTextures();
    Fonts::initFonts();
    sf::Clock deltaClock;

    LOG("setup window");
    float cameraShakeRange = 0.0f;

    window.setFramerateLimit(60); // to avoid pc flying into space
    window.setView(defaultView);

    sf::RectangleShape fadeRect(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    fadeRect.setPosition({0,0});
    fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
    bool fadeActive = false;
    int fadeValue = 0;
    int fadeTarget = 0;
    float fadeSpeed = 500;
    sf::Vector2f teleportTargetPos;

    LOG("load tilemap");
    TileMap tileMap(20, 20, 80.f);

    LOG("initializing player");
    Player player(window);
    LOG("player object created");
    PlayerHealthBar playerHealthBar({420, 40}, 100);
    LOG("initializing inventoryInterface");
    InventoryInterface inventoryInterface(&player.inventory, {1025.f, 700.f}, {(float)window.getSize().x / 2, (float)window.getSize().y / 2});
    bool inventoryToggled = false;
    float lastBulletReloadDelay = .0f;

    LOG("TEST: Item and inventory stuff");
    // ! TEST
    Item grassBfr(false, false, false, 0, 0, Textures::get(Textures::TextureType::Grass));
    player.inventory.setItem({1, 1}, &grassBfr);

    LootContainer chest(player.hitbox.position, {100.f, 75.f}, 125.f);
    // !

    LOG("Initializing shoot mechanics");
    BulletMeter bulletMeter(sf::Vector2f(6, 50), 50);

    float fireRate = 20.0f;
    float timeSinceLastShot = 0.0f;

    LOG("Spawning zombies");
    for (int i = 0; i < 10; i++) {
        new Zombie({(float)(rand() % 800), 0}, &player);
    }

    // DOOR TEST
    LOG("TEST: Doors");
    Door* doorA = new Door(Hitbox({200, 200}, {80, 200}));
    Door* doorB = new Door(Hitbox({700, 200}, {80, 200}));

    doorA->targetDoor = doorB;
    doorB->targetDoor = doorA;

    LOG("Starting game loop");
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::Resized>()) {
                sf::Vector2f newSize = {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)};

                defaultView.setSize({newSize.x, newSize.y});
                defaultView.setCenter({newSize.x / 2, newSize.y / 2});
                player.view.setSize({newSize.x, newSize.y});
                inventoryInterface.resizeBackground(newSize);
                fadeRect.setSize(newSize);
                // player.inventory.resizeForeground({ newSize.x * 0.75f, newSize.y * 0.75f });
                inventoryInterface.setPosition({newSize.x / 2, newSize.y / 2});
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
                    window.close();
                if (keyPressed->scancode == sf::Keyboard::Scan::I) {
                    inventoryToggled = !inventoryToggled;
                }
                if(keyPressed->scancode == sf::Keyboard::Scan::X)
                { 
                    for(int i = 0; i < Door::pool.size(); i++)
                    {
                        if(Door::pool[i]->hitbox.withinBounds(player.sprite.getPosition()))
                        {
                            fadeActive = true;
                            fadeTarget = 255;
                            teleportTargetPos = sf::Vector2f(
                                Door::pool[i]->targetDoor->hitbox.position.x + Door::pool[i]->targetDoor->hitbox.size.x/2 - player.sprite.getSize().x/2,
                                Door::pool[i]->targetDoor->hitbox.position.y + Door::pool[i]->targetDoor->hitbox.size.y - player.sprite.getSize().y);
                            break;
                        }
                    }
                }
            }
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                // handle inventory mouse press
                if (mouseButtonPressed->button == sf::Mouse::Button::Left && inventoryToggled) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(Window::getMousePos());
                    inventoryInterface.handleMousePress(mousePos);
                }
            }
        }

        if (!inventoryToggled) {
            if(!fadeActive)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                    player.move({-player.speed * Physics::deltaTime, 0});
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                    player.move({player.speed * Physics::deltaTime, 0});
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                    player.move({0, -player.speed * Physics::deltaTime});
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                    player.move({0, player.speed * Physics::deltaTime});
                }
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                if (timeSinceLastShot >= (1.0f / fireRate) && !player.reloading) {
                    sf::Vector2i mousePos = Window::getMousePos();
                    float angle = std::atan2(mousePos.y - defaultView.getSize().y / 2, mousePos.x - defaultView.getSize().x / 2);
                    Bullet* b = new Bullet({player.sprite.getPosition().x + player.sprite.getSize().x / 2, player.sprite.getPosition().y + player.sprite.getSize().y / 2}, 2000, angle);
                    timeSinceLastShot = 0.0f;
                    cameraShakeRange = 3.0f;
                    
                    if(bulletMeter.currentBullets > 0)
                    {
                        bulletMeter.setCurrentBullets(bulletMeter.currentBullets - 1);
                        bulletMeter.ejectBullet(bulletMeter.maxBullets - bulletMeter.currentBullets - 1);
                        if(bulletMeter.currentBullets <= 0)
                        {
                            player.reloading = true;
                        }
                    }
                }
            }
            else
            {
                cameraShakeRange = 0; // TODO
            }
        }
        timeSinceLastShot += Physics::deltaTime;

        if(player.reloading)
        {
            lastBulletReloadDelay += Physics::deltaTime;

            if(lastBulletReloadDelay > 0.02f)
            {
                bulletMeter.currentBullets += 1;
                bulletMeter.sprites[bulletMeter.maxBullets - bulletMeter.currentBullets].setFillColor(sf::Color(255, 255, 255, 180));
                lastBulletReloadDelay = .0f;
                if(bulletMeter.currentBullets == bulletMeter.maxBullets)
                {
                    player.reloading = false;
                    for(int i = 0; i < bulletMeter.maxBullets; i++)
                    {
                        bulletMeter.sprites[i].setFillColor(sf::Color::White);
                    }
                }
            }
        }

        Bullet::updateAll();
        player.update();
        Zombie::updateAll();
        Particle::updateAll();
        chest.update(player);

        if (inventoryToggled) {
            inventoryInterface.update();
        }

        window.clear(sf::Color::Black);

        // draw Camera (View)
        window.setView(player.view);
        
        tileMap.draw(window);
        for(auto& it : Door::pool)
            it->debugDraw(window);
        Particle::drawOnlyNonActive(window);
        Bullet::drawAll(window);
        Zombie::drawAll(window);
        player.draw(window);
        player.hitbox.debugDraw(window);
        Particle::drawOnlyActive(window);
        chest.draw();
        DamageIndicatorText::drawAll(window);

        // draw UI
        window.setView(defaultView);
        playerHealthBar.draw(window);

        bulletMeter.updateAnimations();
        bulletMeter.draw(window);

        if (inventoryToggled) {
            inventoryInterface.draw();
        }
        
        if(fadeActive)
        {
            if(fadeValue < fadeTarget)
                fadeValue = std::min(fadeValue + (int)(fadeSpeed * Physics::deltaTime), 255);
            else if(fadeValue > fadeTarget)
            {
                fadeValue = std::max(fadeValue - (int)(fadeSpeed * Physics::deltaTime), 0);
                if(fadeValue == 0)
                    fadeActive = false;
            }
            
            if(fadeValue == 255 && fadeTarget == 255)
            {
                fadeTarget = 0;
                player.setPosition(teleportTargetPos);
            }

            fadeRect.setFillColor(sf::Color(0, 0, 0, fadeValue));
            window.draw(fadeRect);
        }

        if(cameraShakeRange > 0.01f)
            player.view.setCenter({player.view.getCenter().x + randomFloat(-cameraShakeRange, cameraShakeRange), player.view.getCenter().y + randomFloat(-cameraShakeRange, cameraShakeRange)});

        window.display();

        sf::Time dt = deltaClock.restart();
        Physics::deltaTime = dt.asSeconds();
    }

    return 0;
}
