#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "../Window.hpp"
#include "../Physics.hpp"
#include <iostream>

typedef struct
{
    float yAccel;
    float floorTime;
} ParticleSettings;

class Particle
{
public:
    static std::vector<Particle*> pool;
    static void spawnBloodParticles(sf::Vector2f position, int amount, int speedRange);
    static void drawAll(sf::RenderWindow& window);
    static void drawOnlyActive(sf::RenderWindow& window);
    static void drawOnlyNonActive(sf::RenderWindow& window);
    static void updateAll();

    sf::RectangleShape sprite;
    float time; // Amount of time left before the particle despawns
    bool finished;
    sf::Vector2f velocity;
    ParticleSettings settings;
    bool toDelete;

    Particle(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size, sf::Color color, float gravity, float time, float floorTime);

    void update();
    void draw(sf::RenderWindow& window);
};