#include "looting/Item.hpp"
#include <iostream>

Item::Item(std::string name, std::string desc, bool _canDealMeleeDamage, bool _canDealRangedDamage, bool _isHealthPack, int _damage, float _useRate, sf::Texture* _texture) {
    this->_name = name;
    this->_description = desc;
    this->_canDealMeleeDamage = _canDealMeleeDamage;
    this->_canDealRangedDamage = _canDealRangedDamage;
    this->_isHealthPack = _isHealthPack;
    this->_damage = _damage;
    this->_useRate = _useRate;
    this->_texture = _texture;
}

std::string Item::getName() const { return this->_name; }

std::string Item::getDescription() const { return this->_description; }

bool Item::canDealMeleeDamage() const { return _canDealMeleeDamage; }

bool Item::canDealRangedDamage() const { return _canDealRangedDamage; }

bool Item::isHealthPack() const { return _isHealthPack; }

int Item::getDamage() const { return _damage; }

float Item::getUseRate() const { return _useRate; }

sf::Texture* Item::getTexture() const { return _texture; }

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

void Item::setTexture(sf::Texture* texture) {
    _texture = texture;
}