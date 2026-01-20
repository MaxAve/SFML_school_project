#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "Window.hpp"
#include "Player.hpp"
#include "Physics.hpp"
#include "Utils.hpp"
#include "HealthBar.hpp"
#include "Particle.hpp"

class Zombie
{
public:
    static std::vector<Zombie*> pool;
    inline const static float MIN_CROWD_DISTANCE = 80.0f;
    inline const static float CROWD_DISPLACMENET_STRENGTH = 0.2f;
    static void pushAwayFromOthers(); // Get pushed away from other zombies to make sure they don't converge to the same spot
    static void updateAll();
    static void drawAll(sf::RenderWindow& window);

    sf::RectangleShape sprite;
    sf::Vector2f velocity;
    sf::Vector2f displacementVelocity;
    float speed;
    Player* targetPlayer;
    float hitboxRadius;
    HealthBar healthBar;

    Zombie(sf::Vector2f position, Player* targetPlayer);
    void update();
    void draw(sf::RenderWindow& window);
    sf::Vector2f getHitboxPosition();
};
