#include "looting/Item.hpp"
#include <iostream>

Item::Item(ItemType itemType, std::string name, std::string desc, size_t amount, bool _canDealMeleeDamage, bool _canDealRangedDamage, bool _isHealthPack, int _damage, float _useRate, sf::Texture* _texture) {
    this->_type = itemType;
    this->_actualAmount = amount;

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

ItemType Item::getType() const {
    return _type;
}

size_t Item::getMaximalAmount() const {
    return _maximalAmount;
}

size_t Item::getAmount() const {
    return _actualAmount;
}

void Item::setAmount(size_t val) {
    if (val > _maximalAmount) {
        _actualAmount = _maximalAmount;
        return;
    }

    _actualAmount = val;
}

bool Item::isFull() const {
    return _actualAmount == _maximalAmount;
}

/**
 * The function `addAmount` in C++ adds a specified amount to an item, considering the maximum amount
 * that can be added.
 *
 * @param val The `val` parameter in the `addAmount` function represents the amount of items that you
 * want to add to the current amount of items stored in the `Item` object.
 *
 * @return The function `addAmount` returns the amount that was not able to be added to the item due to
 * space constraints.
 */
int Item::addAmount(int val) {
    if (val < 0) {
        int amountToRemove = std::min(static_cast<int>(_actualAmount), -val);
        _actualAmount -= amountToRemove;
        return val + amountToRemove; 
    }

    int spaceLeft = _maximalAmount - _actualAmount;
    int amountToAdd = std::min(spaceLeft, val);

    // swap
    if (!spaceLeft) {
        int prevAmount = _actualAmount;
        _actualAmount = val;

        return prevAmount;
    }

    _actualAmount += amountToAdd;

    return val - amountToAdd;
}

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