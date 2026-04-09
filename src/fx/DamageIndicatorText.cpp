#include "fx/DamageIndicatorText.hpp"

std::vector<DamageIndicatorText *> DamageIndicatorText::pool;

DamageIndicatorText::DamageIndicatorText(sf::Vector2f pos, int damage)
{
    this->timeRemaining = 0.6 + ((float)(rand() % 50) / 100.0f);
    this->text = new sf::Text(Fonts::pixel);
    this->text->setString(std::to_string(damage));
    this->text->setFillColor(sf::Color::White);
    this->text->setCharacterSize(32); 
    this->text->setPosition(pos);
    this->text->setStyle(sf::Text::Bold);
    DamageIndicatorText::pool.push_back(this);
}

void DamageIndicatorText::drawAll(sf::RenderWindow& window)
{
    for(int i = 0; i < DamageIndicatorText::pool.size(); i++)
    {
        window.draw(*(DamageIndicatorText::pool[i]->text));
        DamageIndicatorText::pool[i]->timeRemaining -= Physics::deltaTime;
        DamageIndicatorText::pool[i]->text->setPosition(sf::Vector2f(DamageIndicatorText::pool[i]->text->getPosition().x, DamageIndicatorText::pool[i]->text->getPosition().y - 20 * Physics::deltaTime));
        if(DamageIndicatorText::pool[i]->timeRemaining <= 0.0f)
        {
            DamageIndicatorText* ptr = DamageIndicatorText::pool[i];
            DamageIndicatorText::pool.erase(DamageIndicatorText::pool.begin() + i);
            delete ptr->text;
            delete ptr;
            i--;
        }
    }
}