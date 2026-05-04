#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

enum class ItemType {
    SMALL_CALIBER_AMMO,
    SHOTGUN_AMMO,
    LARGE_CALIBER_AMMO,
    MEDIUM_CALIBER_AMMO,
    BANDAGE,
    MEDKIT,
    KITCHEN_KNIFE,
    COMBAT_KNIFE,
    LOCKPICK,
    SCOPE,
    GUN_SMG,
    GUN_AR,
};

struct ItemData {
    std::string name;
    std::string description;
    bool canDealMeleeDamage;  // true if the item is a close-range weapon like a knife
    bool canDealRangedDamage; // true if the item can fire bullets
    bool isHealthPack;        // true if the item can be used to heal the player
    int damage;               // Note: if ```isHealthPack``` is true, this attribute will be used to determine how much health the player gains
    float useRate;            // how many times the item can be used per second
    std::string texturePath;     // texture to use when displaying the item
    size_t maximalAmount;
};

class Item {
    ItemType type;
    const ItemData& data;
    size_t amount = 1;

    static std::unordered_map<ItemType, ItemData> typeToData;

public:
    Item() = default;

    Item(ItemType type, size_t amount);

    std::string getName() const;

    std::string getDescription() const;

    bool canDealMeleeDamage() const;

    bool canDealRangedDamage() const;

    bool isHealthPack() const;

    int getDamage() const;

    float getUseRate() const;

    ItemType getType() const;

    size_t getMaximalAmount() const;

    size_t getAmount() const;

    void setAmount(size_t val);

    bool isFull() const;

    int addAmount(int val);

    sf::Texture* getTexture() const;
};