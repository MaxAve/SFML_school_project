#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Physics.h"
#include "Zombie.h"

class Bullet
{
public:
	static std::vector<Bullet*> pool; // Should contain every bullet instance
	static constexpr float MAX_RANGE = 1000.0; // Bullets should despawn once they traveled beyond this distance
	
	static void updateAll(); // Updates every bullet in the pool
	static void drawAll(sf::RenderWindow& window);

    const float DEFAULT_SIZE = 20;

    sf::RectangleShape sprite;
    sf::Vector2f velocity;
	float speed;
	float distanceTraveled; // The distance traveled from the bullet's starting position
    sf::Vector2f prevPosition;

    Bullet(sf::Vector2f position, float speed, float direction);
    void draw(sf::RenderWindow& window);
    void update();
    bool hit(Zombie* zombie); // Returns true if the bullet touched or went through the zombie in the current frame
};

#endif
