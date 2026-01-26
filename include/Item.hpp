#pragma once
#include <SFML/Graphics.hpp>

class Item {
    bool _canDealMeleeDamage; // true if the item is a close-range weapon like a knife
    bool _canDealRangedDamage; // true if the item can fire bullets
    bool _isHealthPack; // true if the item can be used to heal the player
    int _damage; // Note: if ```isHealthPack``` is true, this attribute will be used to determine how much health the player gains
    float _useRate; // how many times the item can be used per second
    sf::Texture _texture; // texture to use when displaying the item

public:
    Item() = default;

    Item(bool _canDealMeleeDamage, bool _canDealRangedDamage, bool _isHealthPack, int _damage, float _useRate, sf::Texture _texture);

    Item(bool _canDealMeleeDamage, bool _canDealRangedDamage, bool _isHealthPack, int _damage, float _useRate, const char* pathToTexture);

    bool canDealMeleeDamage() const;

    bool canDealRangedDamage() const;

    bool isHealthPack() const;

    int getDamage() const;

    float getUseRate() const;

    const sf::Texture& getTexture() const;

    void setCanDealMeleeDamage(bool val);

    void setCanDealRangedDamage(bool val);

    void setIsHealthPack(bool val);

    void setDamage(int val);

    void setUseRate(float val);

    void setTexture(const char* pathToTexture);

    void setTexture(sf::Texture texture);
};