#include "Item.hpp"
#include <iostream>

Item::Item(bool _canDealMeleeDamage, bool _canDealRangedDamage, bool _isHealthPack, int _damage, float _useRate, sf::Texture _texture)
: _texture(std::move(_texture))
{
    this->_canDealMeleeDamage = _canDealMeleeDamage;
    this->_canDealRangedDamage = _canDealRangedDamage;
    this->_isHealthPack = _isHealthPack;
    this->_damage = _damage;
    this->_useRate = _useRate;
}

Item::Item(bool _canDealMeleeDamage, bool _canDealRangedDamage, bool _isHealthPack, int _damage, float _useRate, const char* pathToTexture) {
    this->_canDealMeleeDamage = _canDealMeleeDamage;
    this->_canDealRangedDamage = _canDealRangedDamage;
    this->_isHealthPack = _isHealthPack;
    this->_damage = _damage;
    this->_useRate = _useRate;

    if (!_texture.loadFromFile(pathToTexture)) {
        std::cout << "Error: Item::Item(bool, bool, bool, int, float, const char*); Couldn't load file" << std::endl;
    }
}

bool Item::canDealMeleeDamage() const { return _canDealMeleeDamage; }

bool Item::canDealRangedDamage() const { return _canDealRangedDamage; }

bool Item::isHealthPack() const { return _isHealthPack; }

int Item::getDamage() const { return _damage; }

float Item::getUseRate() const { return _useRate; }

const sf::Texture& Item::getTexture() const { return _texture; }

void Item::setCanDealMeleeDamage(bool val) {
    _canDealMeleeDamage = val;
}

void Item::setCanDealRangedDamage(bool val) {
    _canDealRangedDamage = val;
}

void Item::setIsHealthPack(bool val) {
    _isHealthPack = val;
}

void Item::setDamage(int val) {
    _damage = val;
}

void Item::setUseRate(float val) {
    _useRate = val;
}

void Item::setTexture(const char* pathToTexture) {
    if (!_texture.loadFromFile(pathToTexture)) {
        std::cout << "Error: Item::setTexture(const char*); Couldn't load from file" << std::endl;
    }   
}

void Item::setTexture(sf::Texture texture) {
    _texture = std::move(texture);
}