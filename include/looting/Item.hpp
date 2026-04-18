#pragma once
#include <SFML/Graphics.hpp>
#include <string>

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
};

enum class ItemCategory {
    FIRE_ARM, // ? maybe too abstract
    COLD_WEAPON,
    CONSUMABLE,
    TOOL,
    AMMUNITION,
};

class Item {
    bool _canDealMeleeDamage;  // true if the item is a close-range weapon like a knife
    bool _canDealRangedDamage; // true if the item can fire bullets
    bool _isHealthPack;        // true if the item can be used to heal the player
    int _damage;               // Note: if ```isHealthPack``` is true, this attribute will be used to determine how much health the player gains
    float _useRate;            // how many times the item can be used per second
    sf::Texture* _texture;     // texture to use when displaying the item
    std::string _name;
    std::string _description;

    ItemCategory _category;
    ItemType _type;
    // ! should depend on item type
    size_t _maximalAmount = 64;
    // ! has to be set in constructor
    size_t _actualAmount = 1;

public:
    Item() = default;

    Item(std::string name, ItemType type, ItemCategory category, std::string desc, bool _canDealMeleeDamage, bool _canDealRangedDamage, bool _isHealthPack, int _damage, float _useRate, sf::Texture* _texture);

    std::string getName() const;

    std::string getDescription() const;

    bool canDealMeleeDamage() const;

    bool canDealRangedDamage() const;

    bool isHealthPack() const;

    int getDamage() const;

    float getUseRate() const;

    ItemType getType() const;

    ItemCategory getCategory() const;

    size_t getMaximalAmount() const;

    size_t getAmount() const;

    void setAmount(size_t val);

    int addAmount(int val);

    sf::Texture* getTexture() const;

    void setCanDealMeleeDamage(bool val);

    void setCanDealRangedDamage(bool val);

    void setIsHealthPack(bool val);

    void setDamage(int val);

    void setUseRate(float val);

    void setTexture(sf::Texture* texture);
};