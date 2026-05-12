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
    GUN_REVOLVER,
};

enum class AmmoType {
    NONE,
    SMALL,
    MEDIUM,
    LARGE,
    SPREAD
};

typedef struct ItemData {
    std::string name;
    std::string description;
    bool canDealMeleeDamage;  // true if the item is a close-range weapon like a knife
    bool isGun;               // true if the item is a gun (what a surpise)
    bool isHealthPack;        // true if the item can be used to heal the player
    int damage;               // Note: if ```isHealthPack``` is true, this attribute will be used to determine how much health the player gains
    float useRate;            // how many times the item can be used per second (= fire rate for guns)
    std::string texturePath;  // texture to use when displaying the item
    size_t maximalAmount;     // Amount of this item that can be stacked in a single inventory slot
    int magSize;              // Magazine size (guns)
    float reloadTime;         // Reload time for a single bullet (guns)
    AmmoType ammoType;        // Caliber required for reloading (guns)
} ItemData_s;

class Item {
    ItemType type;
    const ItemData& data;
    size_t amount = 1;

    static std::unordered_map<ItemType, ItemData> typeToData;

public:
    Item() = default;

    Item(ItemType type, size_t amount);

    const ItemData* getData(); // Get full data struct (TODO replace all usage of below getters with this)

    std::string getName() const;

    std::string getDescription() const;

    bool canDealMeleeDamage() const;

    bool isGun() const;

    bool isHealthPack() const;

    int getDamage() const;

    float getUseRate() const;

    ItemType getType() const;

    size_t getMaximalAmount() const;

    size_t getAmount() const;

    int getMagSize() const;

    void setAmount(size_t val);

    bool isFull() const;

    int addAmount(int val);

    sf::Texture* getTexture() const;
};