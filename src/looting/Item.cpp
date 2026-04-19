#include "looting/Item.hpp"
#include "resources/Textures.hpp"
#include <iostream>

const ItemData& Item::getItemData(ItemType type) {
    switch (type) {
    case ItemType::SMALL_CALIBER_AMMO: {
        static const ItemData data{
            "Small Caliber Ammo", "Standard low-caliber ammunition",
            false, false, false,
            0, 0.f,
            Textures::get("items/bullet_small"),
            16};
        return data;
    }

    case ItemType::SHOTGUN_AMMO: {
        static const ItemData data{
            "Shotgun Ammo", "Shells for shotguns",
            false, false, false,
            0, 0.f,
            Textures::get("items/bullet_spread"),
            16};
        return data;
    }

    case ItemType::LARGE_CALIBER_AMMO: {
        static const ItemData data{
            "Large Caliber Ammo", "High-power ammunition",
            false, false, false,
            0, 0.f,
            Textures::get("items/bullet_large"),
            16};
        return data;
    }

    case ItemType::MEDIUM_CALIBER_AMMO: {
        static const ItemData data{
            "Medium Caliber Ammo", "Standard firearm ammunition",
            false, false, false,
            0, 0.f,
            Textures::get("items/bullet_medium"),
            16};
        return data;
    }

    case ItemType::BANDAGE: {
        static const ItemData data{
            "Bandage", "Consumable\nHeals 20 HP on use",
            false, false, true,
            20, 1.f,
            Textures::get("items/bandage"),
            32};
        return data;
    }

    case ItemType::MEDKIT: {
        static const ItemData data{
            "Medkit", "Consumable\nHeals 80 HP on use",
            false, false, true,
            80, 0.5f,
            Textures::get("items/medkit3d"),
            16};
        return data;
    }

    case ItemType::KITCHEN_KNIFE: {
        static const ItemData data{
            "Kitchen Knife", "Weapon\nDeals 20 damage on hit",
            true, false, false,
            20, 2.f,
            Textures::get("items/kitchen_knife"),
            1};
        return data;
    }

    case ItemType::COMBAT_KNIFE: {
        static const ItemData data{
            "Combat Knife", "Weapon\nDeals 30 damage on hit",
            true, false, false,
            30, 1.5f,
            Textures::get("items/combat_knife"),
            1};
        return data;
    }

    case ItemType::LOCKPICK: {
        static const ItemData data{
            "Lockpick", "Tool\nCan open locked doors and boxes",
            false, false, false,
            0, 1.f,
            Textures::get("items/lockpick"),
            10};
        return data;
    }
    }
    
    assert(false); // crash: invalid item type passed
}

Item::Item(ItemType _type, size_t _amount) : data{getItemData(_type)} {
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

sf::Texture* Item::getTexture() const { return data.texture; }