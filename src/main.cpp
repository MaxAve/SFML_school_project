#include "environment/GameMap.hpp"
#include "core/Physics.hpp"
#include "core/Window.hpp"
#include "entities/Bullet.hpp"
#include "entities/EntityRenderer.hpp"
#include "entities/Player.hpp"
#include "entities/Zombie.hpp"
#include "environment/Door.hpp"
#include "environment/TileMapChunk.hpp"
#include "fx/DamageIndicatorText.hpp"
#include "gui/BulletMeter.hpp"
#include "gui/HotbarGui.hpp"
#include "gui/InventoryInterface.hpp"
#include "gui/ItemLabel.hpp"
#include "gui/PlayerHealthBar.hpp"
#include "gui/SharedInventoryInterface.hpp"
#include "looting/Inventory.hpp"
#include "looting/LootContainer.hpp"
#include "resources/Fonts.hpp"
#include "resources/Textures.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#define LOG(message) std::cout << message << std::endl

float randomFloat(float min, float max) {
    static std::random_device rd;  // seed
    static std::mt19937 gen(rd()); // Mersenne Twister RNG
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

    LOG("Loading tileset");
    if (!TileMapChunk::tilesetAtlas.loadFromFile("resources/textures/environment/tilemap.png")) {
        LOG("Failed to load tileset");
        return 1;
    }

    sf::Shader shader;
	if (!shader.loadFromFile("resources/shaders/lighting.glsl", sf::Shader::Type::Fragment)) {
		std::cout << "Failed to load lighting.glsl\n";
	}
    shader.setUniform("amountLightSources", 0);

    ItemLabel::init();

    sf::Text selectedItemLabel(Fonts::pixel);
    
    selectedItemLabel.setOrigin(sf::Vector2f((selectedItemLabel.findCharacterPos(selectedItemLabel.getString().getSize() - 1).x - selectedItemLabel.findCharacterPos(0).x) / 2, 0));
    selectedItemLabel.setFillColor(sf::Color::White);
    selectedItemLabel.setCharacterSize(30);

    // Test chunk
    TileMapChunk testChunk({0, 0});

    // Fill the chunk with some tiles
    for (int x = 0; x < 16; x++)
        for (int y = 0; y < 16; y++)
            testChunk.tiles[y][x] = 0;
    for (int x = 0; x < 16; x++)
        testChunk.tiles[7][x] = 16;
    for (int x = 0; x < 16; x++)
        testChunk.tiles[8][x] = 17;
    testChunk.tiles[8][8] = 18;
    for (int x = 0; x < 16; x++)
        for (int y = 9; y < 13; y++)
            testChunk.tiles[y][x] = 34;
    for (int x = 0; x < 16; x++)
        if (x % 2 == 0)
            testChunk.tiles[10][x] = 33;
    for (int x = 0; x < 16; x++)
        testChunk.tiles[13][x] = 64;

    testChunk.updateTextures();

    // Second layer for tall grass
    TileMapChunk testChunk2({0, 0});
    for (int x = 0; x < 16; x++)
        for (int y = 0; y < 16; y++)
            if ((y < 7 || y >= 13) && (rand() % 3) == 0)
                testChunk2.tiles[y][x] = 80; // tall grass
            else
                testChunk2.tiles[y][x] = 96; // air
    testChunk2.updateTextures();

    sf::RectangleShape fadeRect(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    fadeRect.setPosition({0, 0});
    fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
    bool fadeActive = false;
    int fadeValue = 0;
    int fadeTarget = 0;
    float fadeSpeed = 500;
    sf::Vector2f teleportTargetPos;

    LOG("initializing player");
    Player player(window);
    LOG("creating playerHealthbar");
    PlayerHealthBar playerHealthBar({500, 40}, 100);
    LOG("creating HotbarGui");
    HotbarGui hotbarGui(&player.hotbar, {((float)window.getSize().x - GuiParameters::slotSizeF * player.hotbar.getSize()) / 2.f,
                                         (float)window.getSize().y - GuiParameters::slotSizeF - 7.5f});
    hotbarGui.setMarkedSlot(0);
    LOG("initializing inventoryInterface");
    InventoryInterface inventoryInterface(&player.inventory, &player.hotbar, {1025.f, 700.f}, {(float)window.getSize().x / 2, (float)window.getSize().y / 2});
    bool inventoryToggled = false;
    LOG("initializing sharedInventoryInterface");
    SharedInventoryInterface sharedInventoryInterface({1025.f, 700.f}, {(float)window.getSize().x / 2, (float)window.getSize().y / 2}, &player.inventory, &player.hotbar, nullptr);
    bool sharedInventoryToggled = false;

    float lastBulletReloadDelay = .0f;

    LOG("TEST: Item and inventory stuff");
    // ! TEST

    Item ti0(ItemType::SMALL_CALIBER_AMMO, 32);
    Item ti1(ItemType::SHOTGUN_AMMO, 1);
    Item ti2(ItemType::LARGE_CALIBER_AMMO, 1);
    Item ti3(ItemType::MEDIUM_CALIBER_AMMO, 1);
    Item ti4(ItemType::BANDAGE, 1);
    Item ti5(ItemType::MEDKIT, 1);
    Item ti6(ItemType::KITCHEN_KNIFE, 1);
    Item ti7(ItemType::COMBAT_KNIFE, 1);
    Item ti8(ItemType::LOCKPICK, 1);
    Item ti9(ItemType::MEDKIT, 1);
    Item ti10(ItemType::SCOPE, 1);
    Item ti11(ItemType::MEDIUM_CALIBER_AMMO, 1);
    Item ti12(ItemType::GUN_SMG, 1);
    Item ti13(ItemType::GUN_AR, 1);
    Item ti14(ItemType::GUN_REVOLVER, 1);

    player.hotbar.setItem(0, &ti12);
    player.inventory.setItem({0, 1}, &ti0);
    player.inventory.setItem({1, 1}, &ti1);
    player.inventory.setItem({2, 1}, &ti2);
    player.inventory.setItem({3, 1}, &ti3);
    player.inventory.setItem({4, 1}, &ti4);
    player.inventory.setItem({5, 1}, &ti5);
    player.inventory.setItem({6, 1}, &ti6);
    player.inventory.setItem({7, 1}, &ti7);
    player.inventory.setItem({8, 1}, &ti8);
    player.inventory.setItem({9, 1}, &ti9);
    player.inventory.setItem({0, 2}, &ti10);
    player.inventory.setItem({1, 2}, &ti12);
    player.inventory.setItem({2, 2}, &ti13);
    player.inventory.setItem({3, 2}, &ti14);
    LootContainer chest(player.hitbox.position, {100.f, 75.f}, 125.f);
    // !

    LOG("Initializing shoot mechanics");
    BulletMeter bulletMeter(sf::Vector2f(6, 50), 40);

    float fireRate = 20.0f;
    float timeSinceLastShot = 0.0f;

    LOG("Spawning zombies");
    int nzombies = 10;
    for (int i = 0; i < nzombies; i++) {
        new Zombie({(float)(rand() % 800), 0}, &player);
    }

    Hitbox testHitbox({300, 500}, {100, 100}, true);

    // DOOR TEST
    LOG("TEST: Doors");
    Door* doorA = new Door(Hitbox(sf::Vector2f(200, 200), sf::Vector2f(80, 200)));
    Door* doorB = new Door(Hitbox(sf::Vector2f(700, 200), sf::Vector2f(80, 200)));

    doorA->targetDoor = doorB;
    doorB->targetDoor = doorA;

    LOG("Starting game loop");
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (event->is<sf::Event::Resized>()) {
                sf::Vector2f newSize = {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)};

                defaultView.setSize({newSize.x, newSize.y});
                defaultView.setCenter({newSize.x / 2, newSize.y / 2});
                player.view.setSize({newSize.x, newSize.y});
                inventoryInterface.resizeBackground(newSize);
                sharedInventoryInterface.resizeBackground(newSize);
                fadeRect.setSize(newSize);
                inventoryInterface.setPosition({newSize.x / 2, newSize.y / 2});
                sharedInventoryInterface.setPosition({newSize.x / 2, newSize.y / 2});

                hotbarGui.setPosition({(newSize.x - GuiParameters::slotSizeF * player.hotbar.getSize()) / 2.f,
                                       newSize.y - GuiParameters::slotSizeF - 7.5f});
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
                    window.close();
                else if (keyPressed->scancode == sf::Keyboard::Scan::I && !sharedInventoryToggled && !fadeActive) {
                    inventoryToggled = !inventoryToggled;
                } else if (keyPressed->scancode == sf::Keyboard::Scan::Q && sharedInventoryInterface.getOtherInventory() && !inventoryToggled && !fadeActive) {
                    sharedInventoryToggled = !sharedInventoryToggled;
                } else if (keyPressed->scancode == sf::Keyboard::Scan::X && !sharedInventoryToggled && !inventoryToggled) {
                    for (int i = 0; i < Door::pool.size(); i++) {
                        if (Door::pool[i]->hitbox.withinBounds(player.sprite.getPosition())) {
                            fadeActive = true;
                            fadeTarget = 255;
                            teleportTargetPos = sf::Vector2f(
                                Door::pool[i]->targetDoor->hitbox.position.x + Door::pool[i]->targetDoor->hitbox.size.x / 2 - player.sprite.getSize().x / 2,
                                Door::pool[i]->targetDoor->hitbox.position.y + Door::pool[i]->targetDoor->hitbox.size.y - player.sprite.getSize().y);
                            break;
                        }
                    }
                } else if (!inventoryToggled && !sharedInventoryToggled && keyPressed->code >= sf::Keyboard::Key::Num0 && keyPressed->code <= sf::Keyboard::Key::Num9) {
                    hotbarGui.setMarkedSlot(static_cast<int>(keyPressed->code) - static_cast<int>(sf::Keyboard::Key::Num0) - 1);
                    
                    Item* a = hotbarGui.getSelectedItem();
                    if(a != nullptr)
                    {
                        if(a->getType() == ItemType::GUN_AR || a->getType() == ItemType::GUN_SMG || a->getType() == ItemType::GUN_REVOLVER)
                        {
                            fireRate = a->getUseRate();
                        }
                    }
                }
            } else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                // handle inventory mouse press
                if (mouseButtonPressed->button == sf::Mouse::Button::Left && inventoryToggled) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(Window::getMousePos());
                    inventoryInterface.handleLMB(mousePos);
                }

                else if (mouseButtonPressed->button == sf::Mouse::Button::Left && sharedInventoryToggled) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(Window::getMousePos());
                    sharedInventoryInterface.handleLMB(mousePos);
                }

                else if (mouseButtonPressed->button == sf::Mouse::Button::Right && inventoryToggled) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(Window::getMousePos());
                    inventoryInterface.handleRMB(mousePos);
                }

                else if (mouseButtonPressed->button == sf::Mouse::Button::Right && sharedInventoryToggled) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(Window::getMousePos());
                    sharedInventoryInterface.handleRMB(mousePos);
                }
            }
        }

        shader.setUniform("resolution", sf::Vector2f(window.getSize()));

        // game outside of inventory
        if (!inventoryToggled && !sharedInventoryToggled) {
            if (!fadeActive) {
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

                    if (bulletMeter.currentBullets > 0) {
                        bulletMeter.setCurrentBullets(bulletMeter.currentBullets - 1);
                        bulletMeter.ejectBullet(bulletMeter.maxBullets - bulletMeter.currentBullets - 1);
                        if (bulletMeter.currentBullets <= 0) {
                            player.reloading = true;
                        }
                    }
                }
            } else {
                cameraShakeRange = 0; // TODO
            }
        }
        timeSinceLastShot += Physics::deltaTime;

        if (player.reloading) {
            lastBulletReloadDelay += Physics::deltaTime;

            if (lastBulletReloadDelay > 0.02f) {
                bulletMeter.currentBullets += 1;
                bulletMeter.sprites[bulletMeter.maxBullets - bulletMeter.currentBullets].setFillColor(sf::Color(255, 255, 255, 180));
                lastBulletReloadDelay = .0f;
                if (bulletMeter.currentBullets == bulletMeter.maxBullets) {
                    player.reloading = false;
                    for (int i = 0; i < bulletMeter.maxBullets; i++) {
                        bulletMeter.sprites[i].setFillColor(sf::Color::White);
                        bulletMeter.sprites[i].setOutlineColor(sf::Color(180, 180, 180));
                    }
                }
            }
        }

        selectedItemLabel.setString("");
        Item* hotbarItem = hotbarGui.getSelectedItem();
        if(hotbarItem != nullptr)
        {
            selectedItemLabel.setString(hotbarItem->getName());
            selectedItemLabel.setOrigin(sf::Vector2f((selectedItemLabel.findCharacterPos(selectedItemLabel.getString().getSize() - 1).x - selectedItemLabel.findCharacterPos(0).x) / 2, 0));
            selectedItemLabel.setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y-150));
        }

        // LIGHT TEST
        shader.setUniform("amountLightSources", 2);
        
        shader.setUniform("lightSources[0].position", sf::Vector2f(600.0f, 200.0f));
        shader.setUniform("lightSources[0].color", sf::Vector3f(1.0f, 0.0f, 0.0f));
        shader.setUniform("lightSources[0].range", 100.f);
        shader.setUniform("lightSources[0].intensity", 5.0f);

        shader.setUniform("lightSources[1].position", sf::Vector2f(700.0f, 200.0f));
        shader.setUniform("lightSources[1].color", sf::Vector3f(0.0f, 1.0f, 0.0f));
        shader.setUniform("lightSources[1].range", 100.f);
        shader.setUniform("lightSources[1].intensity", 5.0f);

        Bullet::updateAll();
        player.update();
        Zombie::updateAll();
        Particle::updateAll();

        // TODO: optimization needed to support many lootboxes
        chest.update(player);
        if (chest.isPlayerInRange()) {
            sharedInventoryInterface.setOtherInventory(chest.getInventory());
        } else {
            sharedInventoryInterface.setOtherInventory(nullptr);
        }

        if (!inventoryToggled && !sharedInventoryToggled) {
            hotbarGui.update();
        }

        if (inventoryToggled) {
            inventoryInterface.update();
        }
        if (sharedInventoryToggled) {
            sharedInventoryInterface.update();
        }

        EntityRenderer::sortEntities(&player, Zombie::pool);

        window.clear(sf::Color::Black);

        // draw Camera (View)
        window.setView(player.view);

        mainMap.draw(player.hitbox.position, window, shader);

        chest.draw();
        for (auto& it : Door::pool)
            it->debugDraw(window);
        Particle::drawOnlyNonActive(window);
        Bullet::drawAll(window);

        EntityRenderer::drawAll(window);

        // Zombie::drawAll(window);
        // player.draw(window);
        // player.hitbox.debugDraw(window);
        testHitbox.debugDraw(window);
        Particle::drawOnlyActive(window);
        DamageIndicatorText::drawAll(window);

        // draw UI
        window.setView(defaultView);
        playerHealthBar.draw(window);

        if(hotbarItem != nullptr)
        {
            if(hotbarItem->getType() == ItemType::GUN_AR || hotbarItem->getType() == ItemType::GUN_SMG || hotbarItem->getType() == ItemType::GUN_REVOLVER)
            {
                bulletMeter.updateAnimations();
                bulletMeter.draw(window);
            }
        }

        if (!inventoryToggled && !sharedInventoryToggled) {
            hotbarGui.draw();
        }

        if (inventoryToggled) {
            inventoryInterface.draw();
        }
        if (sharedInventoryToggled) {
            sharedInventoryInterface.draw();
        }

        window.draw(selectedItemLabel);

        ItemLabel::draw(window);

        if (fadeActive) {
            if (fadeValue < fadeTarget)
                fadeValue = std::min(fadeValue + (int)(fadeSpeed * Physics::deltaTime), 255);
            else if (fadeValue > fadeTarget) {
                fadeValue = std::max(fadeValue - (int)(fadeSpeed * Physics::deltaTime), 0);
                if (fadeValue == 0)
                    fadeActive = false;
            }

            if (fadeValue == 255 && fadeTarget == 255) {
                fadeTarget = 0;
                player.setPosition(teleportTargetPos);
            }

            fadeRect.setFillColor(sf::Color(0, 0, 0, fadeValue));
            window.draw(fadeRect);
        }

        if (cameraShakeRange > 0.01f)
            player.view.setCenter({player.view.getCenter().x + randomFloat(-cameraShakeRange, cameraShakeRange), player.view.getCenter().y + randomFloat(-cameraShakeRange, cameraShakeRange)});

        window.display();

        sf::Time dt = deltaClock.restart();
        Physics::deltaTime = dt.asSeconds();
    }

    return 0;
}
