#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "core/Window.hpp"
#include "entities/Player.hpp"
#include "core/Physics.hpp"
#include "core/Utils.hpp"
#include "gui/HealthBar.hpp"
#include "fx/Particle.hpp"
#include "core/Hitbox.hpp"
#include "resources/AudioManager.hpp"
#include "core/Animation.hpp"

class Zombie
{
public:
    static std::vector<Zombie*> pool;
    inline const static float MIN_CROWD_DISTANCE = 80.0f;
    inline const static float CROWD_DISPLACMENET_STRENGTH = 0.2f;
    static void pushAwayFromOthers(); // Get pushed away from other zombies to make sure they don't converge to the same spot
    static void updateAll(sf::Vector2f, const GameMap* map);
    static void drawAll(sf::RenderWindow& window);
    float hurtTimer; // The zombie will be unable to move and will display a specific animation for a short amount of time after getting hit
    float timeSinceBlink; // Zombie will blick while the hurt timer is running
    bool isAggro;

    // sound
    sf::Sound sound;
    float soundTimer = 0.f;
    float attackTimer = 0.0f;

    sf::Sprite sprite;
    Animation animation;

    sf::Vector2f velocity;
    sf::Vector2f displacementVelocity;
    sf::Vector2f bulletPushVelocity; // This velocity is used exclusively for when this zombie gets pushed by a bullet
    float speed;
    Player* targetPlayer;
    HealthBar healthBar;
    
    Hitbox hitbox;
    Hitbox envHitbox;

    Zombie(sf::Vector2f position, Player* targetPlayer);
    void update(sf::Vector2f playerPos, const GameMap* map);
    
    void makeRandomSound(sf::Vector2f playerPos);

    void move(sf::Vector2f delta, const GameMap* map);

    void draw(sf::RenderWindow& window);
    sf::Vector2f getHitboxPosition();

private:
    sf::Texture texture;
};
