#include "fx/DamageIndicatorText.hpp"

std::vector<DamageIndicatorText *> DamageIndicatorText::pool;

DamageIndicatorText::DamageIndicatorText(sf::Vector2f pos, int damage, bool crit)
{
    this->crit = crit;
    this->timeRemaining = 0.6 + ((float)(rand() % 50) / 100.0f);
    this->totalTime = this->timeRemaining;
    this->text = new sf::Text(Fonts::pixel);
    this->text->setString(std::to_string(damage));
    this->text->setFillColor(crit? sf::Color(255, 0, 0) : sf::Color(255, 255, 0));
    this->text->setCharacterSize(40); 
    this->text->setPosition(pos);
    if(crit)
    {
        this->text->setOutlineColor(sf::Color::White);
        this->text->setOutlineThickness(2);
    }
    DamageIndicatorText::pool.push_back(this);
}

void DamageIndicatorText::drawAll(sf::RenderWindow& window)
{
    for(int i = 0; i < DamageIndicatorText::pool.size(); i++)
    {
        window.draw(*(DamageIndicatorText::pool[i]->text));
        DamageIndicatorText::pool[i]->timeRemaining -= Physics::deltaTime;
        DamageIndicatorText::pool[i]->text->setPosition(sf::Vector2f(DamageIndicatorText::pool[i]->text->getPosition().x, DamageIndicatorText::pool[i]->text->getPosition().y - 80 * Physics::deltaTime));
        DamageIndicatorText::pool[i]->text->setFillColor(DamageIndicatorText::pool[i]->crit ? sf::Color(255, 0, 0, (int)(DamageIndicatorText::pool[i]->timeRemaining / DamageIndicatorText::pool[i]->totalTime * 255.f)) : sf::Color(255, 255, 0, (int)(DamageIndicatorText::pool[i]->timeRemaining / DamageIndicatorText::pool[i]->totalTime * 255.f)));
        DamageIndicatorText::pool[i]->text->setOutlineColor(sf::Color(255, 255, 255, (int)(DamageIndicatorText::pool[i]->timeRemaining / DamageIndicatorText::pool[i]->totalTime * 255.f)));
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