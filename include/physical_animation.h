#ifndef PHYSICAL_ANIMATION_H
#define PHYSICAL_ANIMATION_H

#include <vector>
#include <SFML/Graphics.hpp>

typedef struct PAMotionTarget_s
{
	float targetDistance; // Target distance from origin
	int velocityDirection; // Either -1 or 1
} PAMotionTarget_t;

typedef struct PAMotion_s
{
	float startSpeed;
	float acceleration;
	float direction;
	PAMotionTarget_t target;
} PAMotion_t;

class PhysicalAnimation
{
public:
	std::vector<PAMotion_t> cycle;
	int currentMotion;
	bool running;
	float distance; // Distance from origin
	sf::RectangleShape* targetSprite; // Replace with sprite

	PhysicalAnimation(sf::RectangleShape* targetSprite);

	void addMotion(float startSpeed, float targetSpeed, float acceleration, float direction);
	void start();
	void update();
};

#endif
