#include "fx/Particle.hpp"

std::vector<Particle*> Particle::pool;

void Particle::drawAll(sf::RenderWindow& window)
{
    for(int i = 0; i < Particle::pool.size(); i++)
    {
        Particle::pool[i]->draw(window);
    }
}

void Particle::drawOnlyActive(sf::RenderWindow &window)
{
    for(int i = 0; i < Particle::pool.size(); i++)
    {
        if(!Particle::pool[i]->finished)
            Particle::pool[i]->draw(window);
    }
}

void Particle::drawOnlyNonActive(sf::RenderWindow &window)
{
    for(int i = 0; i < Particle::pool.size(); i++)
    {
        if(Particle::pool[i]->finished)
            Particle::pool[i]->draw(window);
    }
}

void Particle::updateAll()
{
    for(int i = 0; i < Particle::pool.size(); i++)
    {
        Particle::pool[i]->update();
        if(Particle::pool[i]->toDelete)
        {
            // TODO memory leak because no delete call
            Particle::pool.erase(Particle::pool.begin() + i);
            i--;
        }
    }
}

void Particle::spawnBloodParticles(sf::Vector2f position, int amount, int speedRange)
{
    for(int i = 0; i < amount; i++)
    {
        new Particle(position, {(float)((rand() % speedRange) - speedRange/2), (float)(rand() % speedRange) - speedRange/2}, {22, 22}, sf::Color(150, 0, 0), 1500, 0.3, 3);
    }
}

Particle::Particle(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size, sf::Color color, float gravity, float time, float floorTime)
{
    this->sprite = sf::RectangleShape(size);
    this->sprite.setPosition(position);
    this->sprite.setFillColor(color);
    this->velocity = velocity;
    this->time = time;
    this->settings.floorTime = floorTime;
    this->settings.yAccel = gravity;
    this->finished = false;
    Particle::pool.push_back(this);
}

void Particle::update()
{
    if(!this->finished)
    {
        this->velocity.y += this->settings.yAccel * Physics::deltaTime;
        this->sprite.move({this->velocity.x * Physics::deltaTime, this->velocity.y * Physics::deltaTime});
        this->time -= Physics::deltaTime;

        if(this->time <= .0f)
        {
            this->finished = true;
            this->sprite.setSize({this->sprite.getSize().x, this->sprite.getSize().y / 2.0f});
        }
    }
    else
    {
        this->settings.floorTime -= Physics::deltaTime;
        if(this->settings.floorTime < .0f)
        {
            this->toDelete = true;
        }
    }
}

void Particle::draw(sf::RenderWindow &window)
{
    window.draw(this->sprite);
}