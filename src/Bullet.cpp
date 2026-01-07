#include "../include/Bullet.h"

std::vector<Bullet*> Bullet::pool;

Bullet::Bullet(sf::Vector2f position, float speed, float direction)
{
	this->speed = speed;

    this->velocity = {std::cos(direction) * speed, std::sin(direction) * speed};

    this->sprite = sf::RectangleShape({Bullet::DEFAULT_SIZE, Bullet::DEFAULT_SIZE});
    this->sprite.setPosition(position);
    this->sprite.setFillColor(sf::Color::Yellow);

	Bullet::pool.push_back(this); // Add this bullet to the pool
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(this->sprite);
}

void Bullet::update()
{
    this->sprite.move({this->velocity.x * Physics::deltaTime, this->velocity.y * Physics::deltaTime});
	this->distanceTraveled += this->speed * Physics::deltaTime; 
}

void Bullet::drawAll(sf::RenderWindow& window)
{
	for(int i = 0; i < Bullet::pool.size(); i++)
	{
		Bullet::pool[i]->draw(window);
	}
}

void Bullet::updateAll()
{
	for(int i = 0; i < Bullet::pool.size(); i++)
		Bullet::pool[i]->update();

	// Delete all bullets that are out of range (Note: this assumes bullets are ordered from most to least travelled distance)
	int i = 0;
	while(i < Bullet::pool.size() && Bullet::pool[i]->distanceTraveled > Bullet::MAX_RANGE)
		i++;
	Bullet::pool.erase(Bullet::pool.begin(), std::next(Bullet::pool.begin(), i));
}
