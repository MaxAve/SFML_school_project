#include "TileMapEditor.hpp"
#include "core/Physics.hpp"
#include "core/Window.hpp"
#include "entities/Bullet.hpp"
#include "entities/EntityRenderer.hpp"
#include "entities/Player.hpp"
#include "entities/Zombie.hpp"
#include "environment/Door.hpp"
#include "environment/GameMap.hpp"
#include "environment/Structure.hpp"
#include "environment/TileMapChunk.hpp"
#include "fx/DamageIndicatorText.hpp"
#include "gui/BulletMeter.hpp"
#include "gui/Button.hpp"
#include "gui/HotbarGui.hpp"
#include "gui/InventoryInterface.hpp"
#include "gui/ItemLabel.hpp"
#include "gui/PlayerHealthBar.hpp"
#include "gui/SharedInventoryInterface.hpp"
#include "looting/Inventory.hpp"
#include "looting/LootContainer.hpp"
#include "resources/AudioManager.hpp"
#include "resources/Fonts.hpp"
#include "resources/Textures.hpp"
#include "core/EntitySpawner.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <string.h>

#define LOG(message) std::cout << "[LOG] " << message << std::endl
#define ERR(message) std::cout << "[ERR] " << message << std::endl

float randomFloat(float min, float max) {
    static std::random_device rd;  // seed
    static std::mt19937 gen(rd()); // Mersenne Twister RNG
    std::uniform_real_distribution<float> dist(min, max);

    return dist(gen);
}

enum class GameState {
    PAUSE,
    MAIN_MENU,
    GAME,
    EXITING,
    GAME_OVER,
};

void runMainMenu(GameState& gameState, sf::Clock& deltaClock) {
    bool debugMode = false;

    enum class Page {
        MAIN,
        CREDITS,
    };

    Page curPage = Page::MAIN;

    sf::Vector2f center = window.getView().getCenter();
    sf::Vector2f buttonSize = {500.f, 75.f};
    sf::Vector2f gapSize = {0.f, 30.f}; // gap between buttons

    sf::Text versionText(Fonts::pixel, "Version: Pre-Pre-Alpha-demo-beta-0.1", 20U);
    versionText.setFillColor(sf::Color::White);
    versionText.setOrigin(versionText.getLocalBounds().size);
    versionText.setPosition(center + window.getView().getSize() / 2.f + sf::Vector2f{-5.f, -10.f});

    // for main Page
    const float TITLE_AMPLITUDE = 10.f;
    const float TITLE_FREQUENCY = 0.5f;
    const float TITLE_PERIOD = 1 / TITLE_FREQUENCY;
    float totalTime = 0.f;
    sf::Vector2f basePosition = center + sf::Vector2f{0.f, -100.f};

    std::array<std::string, 5> randPopUp{
        "Also try C++!", // TODO: ai generated, make own
        "Pixel perfect!",
        "Watch out for zombies!",
        "SFML powered!",
        "Pre-Pre-Alpha-Approved!"};

    sf::Text popUpText(Fonts::pixel, randPopUp[randomFloat(0.f, randPopUp.size() - 1)], 25U);
    popUpText.setFillColor(sf::Color::Yellow);
    popUpText.setRotation(sf::degrees(-20.f));
    popUpText.setOrigin({popUpText.getLocalBounds().getCenter().x, popUpText.getLocalBounds().getCenter().y});

    sf::Sprite gameTitle(*Textures::get("gui_hud/title"));
    gameTitle.setScale({4.f, 4.f});
    gameTitle.setOrigin({gameTitle.getLocalBounds().getCenter().x, gameTitle.getLocalBounds().size.y});
    gameTitle.setPosition(basePosition);

    std::array<Button, 3> mainButtons{
        Button(center, buttonSize, "Start Game", [&gameState]() {
            gameState = GameState::GAME;
        }),
        Button(center + sf::Vector2f{0.f, gapSize.y * 1} + sf::Vector2f{0.f, buttonSize.y * 1}, buttonSize, "Show Credits", [&]() {
            curPage = Page::CREDITS;
        }),
        Button(center + sf::Vector2f{0.f, gapSize.y * 2} + sf::Vector2f{0.f, buttonSize.y * 2}, buttonSize, "Quit Game", [&gameState]() {
            gameState = GameState::EXITING;
        })};

    // for credits page
    std::string creditsStr = R"(
    Programmers: MaxAve, spartancmd
    2D Artist: MaxAve
    Musician: spartancmd
    Sound Designer: spartancmd
    Texture Integrator: MaxAve, spartancmd
    TileMap-Editor creator: MaxAve
    TileMap-Editor operator: MaxAve
    Looting designer: spartancmd
    Testers: spartancmd, MaxAve
    )";

    sf::Text creditsText(Fonts::pixel, creditsStr, 30U);
    creditsText.setFillColor(sf::Color::White);
    creditsText.setOrigin({creditsText.getLocalBounds().getCenter().x, creditsText.getLocalBounds().size.y});
    creditsText.setPosition(center + sf::Vector2f{0.f, (buttonSize.y + gapSize.y) * 2} + sf::Vector2f{0.f, -60.f}); // relativ to "back" button

    std::array<Button, 1> creditButtons{
        Button(center + sf::Vector2f{0.f, gapSize.y * 2} + sf::Vector2f{0.f, buttonSize.y * 2}, buttonSize, "Back", [&]() {
            curPage = Page::MAIN;
        })};

    while (gameState == GameState::MAIN_MENU) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                gameState = GameState::EXITING;
            } else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    if (curPage == Page::MAIN)
                        for (auto& button : mainButtons) {
                            if (!button.isHovered()) {
                                continue;
                            }
                            button.setHovered(false);
                            button.run();
                            break;
                        }
                    else if (curPage == Page::CREDITS)
                        for (auto& button : creditButtons) {
                            if (!button.isHovered()) {
                                continue;
                            }
                            button.setHovered(false);
                            button.run();
                            break;
                        }
                }
            } else if (event->is<sf::Event::Resized>()) {
                sf::Vector2f newSize = {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)};

                // Resize window to fit the new size
                defaultView.setSize({newSize.x, newSize.y});
                defaultView.setCenter({newSize.x / 2, newSize.y / 2});
                center = defaultView.getCenter();

                for (size_t i = 0; i < mainButtons.size(); ++i) {
                    mainButtons[i].setPosition(center + sf::Vector2f{0.f, i * (buttonSize.y + gapSize.y)});
                }
                creditButtons[0].setPosition(center + sf::Vector2f{0.f, 2 * (buttonSize.y + gapSize.y)});

                creditsText.setPosition(center + sf::Vector2f{0.f, (buttonSize.y + gapSize.y) * 2} + sf::Vector2f{0.f, -60.f});

                basePosition = center + sf::Vector2f{0.f, -100.f};
                gameTitle.setPosition(basePosition);

                versionText.setPosition(center + defaultView.getSize() / 2.f + sf::Vector2f{-5.f, -10.f});
            }
        }

        window.clear(sf::Color(106, 73, 49));

        window.setView(defaultView);

        switch (curPage) {
        case Page::MAIN: {
            totalTime += Physics::deltaTime;
            float deltaS = TITLE_AMPLITUDE * cos(2 * M_PI * TITLE_FREQUENCY * totalTime);
            gameTitle.setPosition(basePosition + sf::Vector2f{0.f, deltaS});

            if (totalTime >= TITLE_PERIOD) {
                totalTime -= TITLE_PERIOD;
            }

            float scale = 1.0f + 0.12f * std::sin(totalTime * 9.f);
            popUpText.setScale({scale, scale});

            float halfTitleWidth = gameTitle.getGlobalBounds().size.x / 2.f;
            popUpText.setPosition(gameTitle.getPosition() + sf::Vector2f{halfTitleWidth - 20.f, -15.f});

            if (totalTime >= TITLE_PERIOD) {
                totalTime -= TITLE_PERIOD;
            }

            for (auto& button : mainButtons) {
                button.update(static_cast<sf::Vector2f>(Window::getMousePos()));
            }

            window.draw(gameTitle);
            window.draw(popUpText);

            for (auto& button : mainButtons) {
                button.draw();
            }

            break;
        }
        case Page::CREDITS: {
            window.draw(creditsText);

            for (auto& button : creditButtons) {
                button.update(static_cast<sf::Vector2f>(Window::getMousePos()));
            }

            for (auto& button : creditButtons) {
                button.draw();
            }
            break;
        }
        default:
            curPage = Page::MAIN;
        }

        window.draw(versionText);

        sf::Time dt = deltaClock.restart();
        Physics::deltaTime = dt.asSeconds();

        window.display();
    }
}

// ! possible memory leaks
void runGameplay(GameState& gameState, sf::View& defaultView, sf::Shader& shader, sf::Clock& deltaClock, bool& debugMode, float& cameraShakeRange) {
    // rather to the gameplay func

    // Item label
    sf::Text selectedItemLabel(Fonts::pixel);
    selectedItemLabel.setOrigin(sf::Vector2f((selectedItemLabel.findCharacterPos(selectedItemLabel.getString().getSize() - 1).x - selectedItemLabel.findCharacterPos(0).x) / 2, 0));
    selectedItemLabel.setFillColor(sf::Color::White);
    selectedItemLabel.setCharacterSize(30);

    GameMap mainMap({0, 0}, "map/map_layer1.bin", "map/map_layer2.bin", 16, 16);

    // Values for fading out effect
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
    player.setPosition(sf::Vector2f(1500, 1000));
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
    float rmbCooldownTimer = 0.f;
    const float RMB_DELAY = 0.10f;

    float lastBulletReloadDelay = .0f;

    // TEST: items
    LOG("TEST: Item and inventory stuff");
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
    // !

    LOG("Initializing shoot mechanics");
    BulletMeter bulletMeter(sf::Vector2f(6, 50), 40);

    float timeSinceLastShot = 0.0f;
    int weaponDamage = 0;

    // TEST: Spawn zombies
    LOG("Spawning zombies");
    //new Zombie({800, 1000}, &player);
    //new Zombie({800, 900}, &player);
    //new Zombie({800, 800}, &player);

    Hitbox testHitbox({300, 500}, {100, 100}, true);

    // Environment TEST
    LOG("TEST: Environment stuff");
    // Door* doorA = new Door(0, Hitbox(sf::Vector2f(200, 200), sf::Vector2f(80, 200)));
    // Door* doorB = new Door(1, Hitbox(sf::Vector2f(700, 200), sf::Vector2f(80, 200)));

    // LootContainer* chest = LootContainer::create(0, player.hitbox.position + sf::Vector2f{300.f, 0.f}, sf::Vector2f{100.f, 75.f}, 125.f);

    // doorA->targetDoor = doorB;
    // doorB->targetDoor = doorA;

    // Structure* structure = Structure::create(0, sf::Vector2f{900, 750});

    GameMap::loadEnvironment("map/environment.bin");

    // resizement (if it was done previously)
    sf::Vector2f newSize = window.getView().getSize();

    // Resize window along with some GUI elements to fit the new size
    player.view.setSize({newSize.x, newSize.y});
    inventoryInterface.resizeBackground(newSize);
    sharedInventoryInterface.resizeBackground(newSize);
    fadeRect.setSize(newSize);
    inventoryInterface.setPosition({newSize.x / 2, newSize.y / 2});
    sharedInventoryInterface.setPosition({newSize.x / 2, newSize.y / 2});

    hotbarGui.setPosition({(newSize.x - GuiParameters::slotSizeF * player.hotbar.getSize()) / 2.f,
                           newSize.y - GuiParameters::slotSizeF - 7.5f});

    // TODO this is so that the item that the player equips on game start gets registered. Remove this later
    player.equippedItem = hotbarGui.getSelectedItem();
    if (player.equippedItem != nullptr && player.equippedItem->getData()->isGun)
        bulletMeter.initSprites(player.equippedItem->getData()->magSize);

    sf::Vector2f playerVelocity(0, 0);

    sf::Vector2f center = defaultView.getCenter();
    sf::Vector2f buttonSize = {500.f, 75.f};
    sf::Vector2f gapSize = {0.f, 30.f}; // gap between buttons

    std::array<Button, 2> pauseButtons{
        Button(center, buttonSize, "Resume", [&gameState]() {
            gameState = GameState::GAME;
        }),
        Button(center + sf::Vector2f{0.f, gapSize.y} + sf::Vector2f{0.f, buttonSize.y * 1}, buttonSize, "Return to menu", [&gameState]() {
            gameState = GameState::MAIN_MENU;
        })};
    sf::RectangleShape transparentForeground(newSize);
    transparentForeground.setFillColor(sf::Color(20, 20, 20, 230));
    transparentForeground.setOrigin(transparentForeground.getLocalBounds().getCenter());
    transparentForeground.setPosition(center);

    float spawnTimer = 0.0f;

    AudioManager::playBackground("background_night");
    AudioManager::playRandomMusic(); // Game loop
    LOG("Starting game loop");
    while (gameState == GameState::GAME || gameState == GameState::PAUSE) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                gameState = GameState::EXITING;
            } else if (event->is<sf::Event::Resized>()) {
                sf::Vector2f newSize = {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)};

                // Resize window along with some GUI elements to fit the new size
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

                center = defaultView.getCenter();
                for (size_t i = 0; i < pauseButtons.size(); ++i) {
                    pauseButtons[i].setPosition(center + sf::Vector2f{0.f, i * (buttonSize.y + gapSize.y)});
                }
                transparentForeground.setSize(newSize);
                transparentForeground.setOrigin(transparentForeground.getLocalBounds().getCenter());
                transparentForeground.setPosition(center);
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::F3) {
                    std::cout << "[LOG] Toggle debug mode\n";
                    debugMode = !debugMode;
                }

                /*
                Key press events
                */
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
                    if (gameState == GameState::GAME) {
                        gameState = GameState::PAUSE;
                        AudioManager::stopMusic();
                        LOG("Game Paused");
                    } else if (gameState == GameState::PAUSE) {
                        gameState = GameState::GAME;
                        AudioManager::playRandomMusic();
                        LOG("Game Resumed");
                    }
                } else if (keyPressed->scancode == sf::Keyboard::Scan::I && !sharedInventoryToggled && !fadeActive) {
                    if (inventoryToggled) {
                        inventoryInterface.dropCarriedItem(player.hitbox.position);
                    }
                    inventoryToggled = !inventoryToggled;
                } else if (keyPressed->scancode == sf::Keyboard::Scan::Q && sharedInventoryInterface.getOtherInventory() && !inventoryToggled && !fadeActive) {
                    if (sharedInventoryToggled) {
                        sharedInventoryInterface.dropCarriedItem(player.hitbox.position);
                    }
                    sharedInventoryToggled = !sharedInventoryToggled;
                } else if (keyPressed->scancode == sf::Keyboard::Scan::X && !sharedInventoryToggled && !inventoryToggled) {
                    // Entering doors (teleports player to the door's target door)
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
                    // Selecting hotbar slot
                    hotbarGui.setMarkedSlot(static_cast<int>(keyPressed->code) - static_cast<int>(sf::Keyboard::Key::Num0) - 1);

                    // Set player's equipped item
                    player.equippedItem = hotbarGui.getSelectedItem();
                    if (player.equippedItem != nullptr && player.equippedItem->getData()->isGun)
                        bulletMeter.initSprites(player.equippedItem->getData()->magSize); // Initialize/reset bullet meter if the player equipped a gun
                }
            } else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (gameState == GameState::PAUSE) {
                    for (auto& button : pauseButtons) {
                        if (!button.isHovered()) {
                            continue;
                        }
                        button.setHovered(false);
                        button.run();
                        break;
                    }

                }
                // handle inventory mouse press
                else if (mouseButtonPressed->button == sf::Mouse::Button::Left && inventoryToggled) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(Window::getMousePos());
                    inventoryInterface.handleLMB(mousePos, player.hitbox.position);
                }

                else if (mouseButtonPressed->button == sf::Mouse::Button::Left && sharedInventoryToggled) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(Window::getMousePos());
                    sharedInventoryInterface.handleLMB(mousePos, player.hitbox.position);
                }
            }
        }

        // Update shader resolution
        shader.setUniform("resolution", sf::Vector2f(window.getSize()));

        if (gameState != GameState::PAUSE && gameState != GameState::MAIN_MENU) { // != MAIN_MENU to avoid shooting when leaving the game
            // game outside of inventory
            if (!inventoryToggled && !sharedInventoryToggled) {
                // Player movement
                if (!fadeActive) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                        playerVelocity.x = -player.speed;
                        // player.move({-player.speed * Physics::deltaTime, 0}, &mainMap);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                        playerVelocity.x = player.speed;
                        // player.move({player.speed * Physics::deltaTime, 0}, &mainMap);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                        playerVelocity.y = -player.speed;
                        // player.move({0, -player.speed * Physics::deltaTime}, &mainMap);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                        playerVelocity.y = player.speed;
                        // player.move({0, player.speed * Physics::deltaTime}, &mainMap);
                    }
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                    // Shooting
                    if (player.equippedItem != nullptr && timeSinceLastShot >= (1.0f / player.equippedItem->getData()->useRate) && !player.reloading) {
                        // Calculate firing angle and spawn new bullet instance
                        sf::Vector2i mousePos = Window::getMousePos();
                        float angle = std::atan2(mousePos.y - defaultView.getSize().y / 2, mousePos.x - defaultView.getSize().x / 2);
                        Bullet* b = new Bullet({player.sprite.getPosition().x + player.sprite.getSize().x / 2, player.sprite.getPosition().y + player.sprite.getSize().y / 2},
                                               2000, angle, player.equippedItem->getData()->damage);
                        switch (player.equippedItem->getType()) {
                        case ItemType::GUN_AR:
                            AudioManager::playSound("shot_ar");
                            break;
                        case ItemType::GUN_REVOLVER:
                            AudioManager::playSound("shot_pistol");
                            break;
                        case ItemType::GUN_SMG:
                            AudioManager::playSound("shot_ar");
                            break;
                        default:
                            AudioManager::playSound("shot_ar");
                        }
                        timeSinceLastShot = 0.0f;
                        cameraShakeRange = 3.0f;

                        // Remove 1 bullet from the bullet meter
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

            // Reloading gun
            if (player.reloading && player.equippedItem != nullptr) {
                lastBulletReloadDelay += Physics::deltaTime;

                if (lastBulletReloadDelay > player.equippedItem->getData()->reloadTime) {
                    bulletMeter.currentBullets += 1;
                    bulletMeter.sprites[bulletMeter.maxBullets - bulletMeter.currentBullets].setFillColor(sf::Color(255, 255, 255, 180));
                    lastBulletReloadDelay = .0f;
                    AudioManager::playSound("reload_bullet");
                    if (bulletMeter.currentBullets == bulletMeter.maxBullets) {
                        player.reloading = false;
                        for (int i = 0; i < bulletMeter.maxBullets; i++) {
                            bulletMeter.sprites[i].setFillColor(sf::Color::White);
                            bulletMeter.sprites[i].setOutlineColor(sf::Color(180, 180, 180));
                        }
                    }
                }
            }

            // Update item label (inventory)
            selectedItemLabel.setString("");
            Item* hotbarItem = hotbarGui.getSelectedItem();
            if (hotbarItem != nullptr) {
                selectedItemLabel.setString(hotbarItem->getName());
                selectedItemLabel.setOrigin(sf::Vector2f((selectedItemLabel.findCharacterPos(selectedItemLabel.getString().getSize() - 1).x - selectedItemLabel.findCharacterPos(0).x) / 2, 0));
                selectedItemLabel.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y - 150));
            }

            // LIGHT TEST
            // shader.setUniform("amountLightSources", 2);

            // shader.setUniform("lightSources[0].position", sf::Vector2f(600.0f, 200.0f));
            // shader.setUniform("lightSources[0].color", sf::Vector3f(1.0f, 0.0f, 0.0f));
            // shader.setUniform("lightSources[0].range", 100.f);
            // shader.setUniform("lightSources[0].intensity", 5.0f);

            // shader.setUniform("lightSources[1].position", sf::Vector2f(700.0f, 200.0f));
            // shader.setUniform("lightSources[1].color", sf::Vector3f(0.0f, 1.0f, 0.0f));
            // shader.setUniform("lightSources[1].range", 100.f);
            // shader.setUniform("lightSources[1].intensity", 5.0f);

            player.move(playerVelocity * Physics::deltaTime, &mainMap);
            playerVelocity.x = 0;
            playerVelocity.y = 0;

            // Update physics
            Bullet::updateAll();
            player.update();
            sf::Listener::setPosition({player.hitbox.position.x, player.hitbox.position.y, 0.f});
            Zombie::updateAll(player.hitbox.position, &mainMap);
            Particle::updateAll();
            DroppedItem::updateAll(player);
            LootContainer::updateAll(player);

            for (auto* lc : LootContainer::pool) {
                if (lc->isPlayerInRange()) {
                    sharedInventoryInterface.setOtherInventory(lc->getInventory());
                    break;
                }
                sharedInventoryInterface.setOtherInventory(nullptr);
            }

            if (!inventoryToggled && !sharedInventoryToggled) {
                hotbarGui.update();
            }

            if (rmbCooldownTimer > 0.f) // to avoid going into -infinity
                rmbCooldownTimer -= Physics::deltaTime;

            if (inventoryToggled) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && rmbCooldownTimer <= 0.f) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(Window::getMousePos());
                    inventoryInterface.handleRMB(mousePos);
                    rmbCooldownTimer = RMB_DELAY;
                }
                inventoryInterface.update();
            }
            if (sharedInventoryToggled) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && rmbCooldownTimer <= 0.f) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(Window::getMousePos());
                    sharedInventoryInterface.handleRMB(mousePos);
                    rmbCooldownTimer = RMB_DELAY;
                }
                sharedInventoryInterface.update();
            }

            EntityRenderer::sortEntities(&player, Zombie::pool); // Sort enemies so that those higher above are rendered behind those lower on the screen
        } else {
            for (auto& button : pauseButtons) {
                button.update(static_cast<sf::Vector2f>(Window::getMousePos()));
            }
        }

        if(spawnTimer <= 0.0f)
        {
            EntitySpawner::attemptSpawnZombiesWithinPlayerRadius(&player, &mainMap);
            spawnTimer = 3.0f;
        }
        spawnTimer -= Physics::deltaTime;

        /*
        Draw everything
        */

        window.clear(sf::Color::Black);

        // draw Camera (View)
        window.setView(player.view);

        mainMap.draw(window, shader, player.sprite.getPosition(), debugMode);

        LootContainer::drawAll();
        // Structure::drawAll();
        for (auto& it : Door::pool) {
            it->debugDraw(window);
        }
        Particle::drawOnlyNonActive(window);
        Bullet::drawAll(window);
        DroppedItem::drawAll();

        EntityRenderer::drawAll(window, debugMode);

        // Zombie::drawAll(window);
        // player.draw(window);
        // player.hitbox.debugDraw(window);

        testHitbox.debugDraw(window);
        Particle::drawOnlyActive(window);
        DamageIndicatorText::drawAll(window);

        // draw UI
        window.setView(defaultView);
        playerHealthBar.draw(window);

        if (player.equippedItem != nullptr && player.equippedItem->getData()->isGun) {
            if (player.equippedItem->getType() == ItemType::GUN_AR || player.equippedItem->getType() == ItemType::GUN_SMG || player.equippedItem->getType() == ItemType::GUN_REVOLVER) {
                bulletMeter.updateAnimations();
                bulletMeter.draw(window);
            }
        }

        if (!inventoryToggled && !sharedInventoryToggled) {
            hotbarGui.draw();
        }
        window.draw(selectedItemLabel);
        if (inventoryToggled) {
            inventoryInterface.draw();
            ItemLabel::draw(window);
        }
        if (sharedInventoryToggled) {
            sharedInventoryInterface.draw();
            ItemLabel::draw(window);
        }

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

        if (gameState == GameState::PAUSE) {
            window.draw(transparentForeground);
            for (auto& button : pauseButtons) {
                button.draw();
            }
        }

        if (cameraShakeRange > 0.01f)
            player.view.setCenter({player.view.getCenter().x + randomFloat(-cameraShakeRange, cameraShakeRange), player.view.getCenter().y + randomFloat(-cameraShakeRange, cameraShakeRange)});

        window.display();

        sf::Time dt = deltaClock.restart();
        Physics::deltaTime = dt.asSeconds();
    }

    GameMap::saveEnvironment("map/environment.bin");

    AudioManager::stopMusic();
    AudioManager::stopBackground();
    // TODO: delete allocated stuff etc.
}

int main(int argc, char** argv) {
    // Tile map editor
    char s[] = "--edit";
    if (argc > 1 && strncmp(argv[1], s, 6) == 0) {
        LOG("Entering edit mode");

        std::string p = "";
        if (argc > 2)
            p = std::string(argv[2]);

        std::string p2 = "";
        if (argc > 3)
            p2 = std::string(argv[3]);

        TileMapEditor::start(p, p, p2, p2);

        return 0;
    }

    // Actual game

    // presets
    GameState gameState = GameState::MAIN_MENU;

    LOG("initializing time, physics, textures, fonts, audio");
    srand(time(NULL));
    Physics::init();
    Textures::initTextures();
    Fonts::initFonts();
    AudioManager::init();
    ItemLabel::init();
    sf::Clock deltaClock;
    bool debugMode = false;

    LOG("setup window");
    float cameraShakeRange = 0.0f;
    window.setFramerateLimit(60); // to avoid pc flying into space
    window.setView(defaultView);

    // Load tileset
    LOG("Loading tileset");
    if (!TileMapChunk::tilesetAtlas.loadFromFile("resources/textures/environment/tilemap.png")) {
        ERR("Failed to load tileset");
        return 1;
    }

    // Load shader
    sf::Shader shader;
    if (!shader.loadFromFile("resources/shaders/lighting.glsl", sf::Shader::Type::Fragment)) {
        ERR("Failed to load lighting.glsl");
    }
    shader.setUniform("amountLightSources", 0);

    // the game
    while (window.isOpen() && gameState != GameState::EXITING) {
        switch (gameState) {
        case GameState::MAIN_MENU:
            LOG("runMainMenu()");
            runMainMenu(gameState, deltaClock);
            break;
        case GameState::GAME:
            LOG("runGameplay()");
            runGameplay(gameState, defaultView, shader, deltaClock, debugMode, cameraShakeRange);
            break;
        default:
            LOG("default");
            gameState = GameState::MAIN_MENU;
            return 1;
        }
    }

    return 0;
}