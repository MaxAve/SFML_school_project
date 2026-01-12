#include "../include/Zombie.h"

std::vector<Zombie*> Zombie::pool;

Zombie::Zombie(sf::Vector2f position, Player* targetPlayer)
{
    this->sprite = sf::RectangleShape(sf::Vector2f(50.f, 100.f));
    this->sprite.setPosition(position);
    this->sprite.setFillColor(sf::Color::Red);
    this->sprite.setOutlineThickness(2.0f);
    this->sprite.setOutlineColor(sf::Color(100, 0, 0));
    
    this->speed = 200.0f + (float)(rand() % 50);
    this->velocity = sf::Vector2f(0,0);
    this->displacementVelocity = sf::Vector2f(0,0);
    this->targetPlayer = targetPlayer;
    this->hitboxRadius = 60.0f;
    this->healthBar = HealthBar(100);

    Zombie::pool.push_back(this);
}

void Zombie::pushAwayFromOthers()
{
    for(int i = 0; i < Zombie::pool.size(); i++)
    {
        Zombie::pool[i]->displacementVelocity.x = 0;
        Zombie::pool[i]->displacementVelocity.y = 0;
        for(int j = 0; j < Zombie::pool.size(); j++)
        {
            if(i == j)
                continue;

            float d = utils::distance(Zombie::pool[i]->sprite.getPosition(), Zombie::pool[j]->sprite.getPosition());
            if(d < MIN_CROWD_DISTANCE)
            {
                float angle = std::atan2(Zombie::pool[i]->sprite.getPosition().y - Zombie::pool[j]->sprite.getPosition().y,
                                         Zombie::pool[i]->sprite.getPosition().x - Zombie::pool[j]->sprite.getPosition().x);
                Zombie::pool[i]->displacementVelocity.x += std::cos(angle) * std::pow(MIN_CROWD_DISTANCE - d, 2) * CROWD_DISPLACMENET_STRENGTH;
                Zombie::pool[i]->displacementVelocity.y += std::sin(angle) * std::pow(MIN_CROWD_DISTANCE - d, 2) * CROWD_DISPLACMENET_STRENGTH;
            }
        }
    }
}

void Zombie::updateAll()
{
    Zombie::pushAwayFromOthers();
    for(int i = 0; i < Zombie::pool.size(); i++)
    {
        Zombie::pool[i]->update();
        if(Zombie::pool[i]->healthBar.currentHealth == 0)
        {
            Zombie::pool.erase(Zombie::pool.begin() + i);
            i--;
        }
    }
}

void Zombie::drawAll(sf::RenderWindow &window)
{
    for(int i = 0; i < Zombie::pool.size(); i++)
    {
        Zombie::pool[i]->draw(window);
    }
}

void Zombie::update()
{
    // Move towards player
    float angle = std::atan2(this->targetPlayer->sprite.getPosition().y - this->sprite.getPosition().y,
                             this->targetPlayer->sprite.getPosition().x - this->sprite.getPosition().x);
    this->velocity.x = std::cos(angle) * this->speed;
    this->velocity.y = std::sin(angle) * this->speed;

    if(utils::distance(this->sprite.getPosition(), this->targetPlayer->sprite.getPosition()) > 60.0f)
    {
        this->sprite.move({(this->velocity.x + this->displacementVelocity.x) * Physics::deltaTime, 
                           (this->velocity.y + this->displacementVelocity.y) * Physics::deltaTime});
    }

    this->healthBar.setPosition({this->sprite.getPosition().x + 25, this->sprite.getPosition().y - 10});
}

void Zombie::draw(sf::RenderWindow &window)
{
    window.draw(this->sprite);
    this->healthBar.draw(window);
}

sf::Vector2f Zombie::getHitboxPosition()
{
    return {this->sprite.getSize().x/2 + this->sprite.getPosition().x, this->sprite.getSize().y/2 + this->sprite.getPosition().y};
}