#include <iostream>
#include <string>
#pragma once
class Player;
class Room;
class Area;
class Item {
private:
	std::string name;
	std::string description;
public:
	Item(const std::string& name, const std::string& desc);
	virtual void Interact(Player& player, Player& enemy, Room* currentRoom, Area& gameWorld);
	std::string GetName() const;
	std::string GetDescription() const;
    public:
    virtual int GetDamage() const;
};
class Weapon : public Item {
private:
    int damage;
    int durability;

public:
    Weapon(const std::string& name, const std::string& desc, int damage, int durability)
        : Item(name, desc), damage(damage), durability(durability) {}

    int GetDamage() const override;
    int GetDurability() const;
    void SetDurability(int newDurability);
    void Interact(Player& player, Player& enemy, Room* currentRoom, Area& gameWorld) override;
    int EnemyAttack(Player& player, Player& enemy);
};
// Example usage:
//Item key("Key", "A shiny key that looks important.");