#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "core/Physics.hpp"
#include "entities/Zombie.hpp"
#include "fx/Particle.hpp"
#include "fx/DamageIndicatorText.hpp"

class Bullet
{
public:
	static std::vector<Bullet*> pool; // Should contain every bullet instance
	static constexpr float MAX_RANGE = 1000.0; // Bullets should despawn once they traveled beyond this distance
	
	static void updateAll(); // Updates every bullet in the pool
	static void drawAll(sf::RenderWindow& window);

    const float DEFAULT_SIZE = 18;
    const float GLOW_SCALE = 1.3f;

    sf::RectangleShape sprite;
    sf::RectangleShape glowSprite;
    sf::Vector2f velocity;
	float speed;
	float distanceTraveled; // The distance traveled from the bullet's starting position
    sf::Vector2f prevPosition;
    int damage;

    Bullet(sf::Vector2f position, float speed, float direction, float damage);
    void draw(sf::RenderWindow& window);
    void update();
    bool hit(Zombie* zombie); // Returns true if the bullet touched or went through the zombie in the current frame
};