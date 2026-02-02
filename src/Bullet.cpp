#include "../include/Bullet.hpp"

std::vector<Bullet*> Bullet::pool;

Bullet::Bullet(sf::Vector2f position, float speed, float direction)
{
	this->speed = speed;

    this->velocity = {std::cos(direction) * speed, std::sin(direction) * speed};

    this->sprite = sf::RectangleShape({Bullet::DEFAULT_SIZE, Bullet::DEFAULT_SIZE});
    this->sprite.setPosition(position);
    this->sprite.setFillColor(sf::Color::Yellow);
	this->sprite.setOrigin({Bullet::DEFAULT_SIZE/2, Bullet::DEFAULT_SIZE/2});

	Bullet::pool.push_back(this); // Add this bullet to the pool
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(this->sprite);
}

void Bullet::update()
{
	this->prevPosition = this->sprite.getPosition();
    this->sprite.move({this->velocity.x * Physics::deltaTime, this->velocity.y * Physics::deltaTime});
	this->distanceTraveled += this->speed * Physics::deltaTime;
	for(int i = 0; i < Zombie::pool.size(); i++)
	{
		if(this->hit(Zombie::pool[i]))
		{
			Zombie::pool[i]->healthBar.setHealth(Zombie::pool[i]->healthBar.currentHealth - 30);
			Zombie::pool[i]->bulletPushVelocity = this->velocity / 5.f;
			this->distanceTraveled = 1000000000;
			this->sprite.move({1000000, 1000000});
			break;
		}
	}
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

bool Bullet::hit(Zombie *zombie)
{
	float a = Utils::distance(zombie->getHitboxPosition(), this->sprite.getPosition());
	if(a < zombie->hitboxRadius)
		return true;
	// TODO: fix
	// float c = Utils::distance(zombie->getHitboxPosition(), this->prevPosition);
	// float b = Utils::distance(this->sprite.getPosition(), this->prevPosition);
	// float s = (a + b + c) / 2;
	// float A = std::sqrt(s * (s-a) * (s-b) * (s-c));
	// float h = 2 * A / b;
	// return h < zombie->hitboxRadius;
	return false;
}