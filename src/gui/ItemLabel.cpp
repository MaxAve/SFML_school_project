#include "gui/ItemLabel.hpp"

bool ItemLabel::visible=false;
sf::Text ItemLabel::itemName(Fonts::pixel);
sf::Text ItemLabel::itemDescription(Fonts::pixel);
sf::RectangleShape ItemLabel::box;
sf::RectangleShape ItemLabel::box2;

void ItemLabel::init()
{
    ItemLabel::itemName.setString("");
    ItemLabel::itemName.setFillColor(sf::Color::White);
    ItemLabel::itemName.setCharacterSize(30);

    ItemLabel::itemDescription.setString("");
    ItemLabel::itemDescription.setFillColor(sf::Color(200, 200, 200));
    ItemLabel::itemDescription.setCharacterSize(25);

    ItemLabel::box.setFillColor(sf::Color::Black);
    ItemLabel::box.setOutlineColor(sf::Color::White);
    ItemLabel::box.setOutlineThickness(2);

    ItemLabel::box2.setFillColor(sf::Color::Black);
}

void ItemLabel::update(std::string name, std::string desc)
{
    ItemLabel::itemName.setString(name);
    ItemLabel::itemDescription.setString(desc);

    const float itemNameWidth = itemName.findCharacterPos(itemName.getString().getSize() - 1).x - itemName.findCharacterPos(0).x;
    const float itemDescriptionWidth = itemDescription.findCharacterPos(itemDescription.getString().getSize() - 1).x - itemDescription.findCharacterPos(0).x;
    int lines=2;
    for(int i = 0; i < itemDescription.getString().getSize(); i++)
        if(itemDescription.getString().getData()[i] == '\n')
            lines++;

    ItemLabel::box.setSize(sf::Vector2f(std::max(itemNameWidth, itemDescriptionWidth) + 40, (float)lines * 30 + 30));
    ItemLabel::box2.setSize(sf::Vector2f(std::max(itemNameWidth, itemDescriptionWidth) + 40 + 8, (float)lines * 30 + 15 + 23));
}

void ItemLabel::draw(sf::RenderWindow& window)
{
    if(ItemLabel::visible)
    {
        const sf::Vector2f mp = (sf::Vector2f)Window::getMousePos();

        const float itemNameWidth = itemName.findCharacterPos(itemName.getString().getSize() - 1).x - itemName.findCharacterPos(0).x;
        const float itemNameHeight = itemName.findCharacterPos(itemName.getString().getSize() - 1).y - itemName.findCharacterPos(0).y;
        
        ItemLabel::box.setPosition({mp.x + 25, mp.y + 25});
        ItemLabel::box2.setPosition({mp.x + 21, mp.y + 21});
        ItemLabel::itemName.setPosition({mp.x + 35, mp.y + 30});
        ItemLabel::itemDescription.setPosition({mp.x + 35, mp.y + itemNameHeight + 45 + 30});

        window.draw(ItemLabel::box2);
        window.draw(ItemLabel::box);
        window.draw(ItemLabel::itemName);
        window.draw(ItemLabel::itemDescription);
    }
}