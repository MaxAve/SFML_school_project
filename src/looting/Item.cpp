#include "looting/Item.hpp"
#include "resources/Textures.hpp"
#include <iostream>

std::unordered_map<ItemType, ItemData> Item::typeToData{
    {ItemType::SMALL_CALIBER_AMMO, {"Small Caliber Ammo", "Standard low-caliber ammunition", false, false, false, 0, 0.f, "items/bullet_small", 60}},
    {ItemType::SHOTGUN_AMMO, {"Shotgun Ammo", "Shells for shotguns", false, false, false, 0, 0.f, "items/bullet_spread", 60}},
    {ItemType::LARGE_CALIBER_AMMO, {"Large Caliber Ammo", "High-power ammunition", false, false, false, 0, 0.f, "items/bullet_large", 10}},
    {ItemType::MEDIUM_CALIBER_AMMO, {"Medium Caliber Ammo", "Standard firearm ammunition", false, false, false, 0, 0.f, "items/bullet_medium", 120}},
    {ItemType::BANDAGE, {"Bandage", "Consumable\nHeals 20 HP on use", false, false, true, 20, 1.f, "items/bandage", 3}},
    {ItemType::MEDKIT, {"Medkit", "Consumable\nHeals 80 HP on use", false, false, true, 80, 0.5f, "items/medkit3d", 1}},
    {ItemType::KITCHEN_KNIFE, {"Kitchen Knife", "Weapon\nDeals 20 damage on hit (15% crit chance)", true, false, false, 20, 2.f, "items/kitchen_knife", 1}},
    {ItemType::COMBAT_KNIFE, {"Combat Knife", "Weapon\nDeals 30 damage on hit (20% crit chance)", true, false, false, 30, 1.5f, "items/combat_knife", 1}},
    {ItemType::LOCKPICK, {"Lockpick", "Tool\nCan open locked doors and boxes", false, false, false, 0, 1.f, "items/lockpick", 1}},
    {ItemType::SCOPE, {"Scope", "Weapon Extension\nIncreases crit chance by +100% when crafted onto a gun", false, false, false, 0, 0.f, "items/scope", 1}},
};

Item::Item(ItemType _type, size_t _amount) : data{typeToData.at(_type)} {
    type = _type;
    setAmount(_amount);
}

std::string Item::getName() const { return data.name; }

std::string Item::getDescription() const { return data.description; }

bool Item::canDealMeleeDamage() const { return data.canDealMeleeDamage; }

bool Item::canDealRangedDamage() const { return data.canDealRangedDamage; }

bool Item::isHealthPack() const { return data.isHealthPack; }

ItemType Item::getType() const {
    return type;
}

size_t Item::getMaximalAmount() const {
    return data.maximalAmount;
}

size_t Item::getAmount() const {
    return amount;
}

void Item::setAmount(size_t val) {
    if (val > data.maximalAmount) {
        amount = data.maximalAmount;
        return;
    }

    amount = val;
}

bool Item::isFull() const {
    return amount == data.maximalAmount;
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
        int amountToRemove = std::min(static_cast<int>(amount), -val);
        amount -= amountToRemove;
        return val + amountToRemove;
    }

    int spaceLeft = data.maximalAmount - amount;
    int amountToAdd = std::min(spaceLeft, val);

    // swap
    if (!spaceLeft) {
        int prevAmount = amount;
        amount = val;

        return prevAmount;
    }

    amount += amountToAdd;

    return val - amountToAdd;
}

int Item::getDamage() const { return data.damage; }

float Item::getUseRate() const { return data.useRate; }

sf::Texture* Item::getTexture() const { return Textures::get(data.texturePath); }