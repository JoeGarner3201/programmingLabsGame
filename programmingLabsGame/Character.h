#include <iostream>
#include <string>
#include <vector>
#include "Item.h"
#include "Room.h"
#pragma once

class Character {
private:
	std::string name;
	int health;
	std::vector<Item*> inventory;
public:
	Character(const std::string& name, int health);
	void TakeDamage(int damage);
	int GetHealth() const;
	void SetHealth(int newHealth);
	void AddItem(Item* item);
	void RemoveItem(Item* item);
	Item* GetItem(const std::string& itemName);
	std::string printInventory();
};
class Player : public Character {
private:
	Room* location;
public:
	Player(const std::string& name, int health);
	void SetLocation(Room* newLocation);
	Room* GetLocation();
	void RoomInteract();
	void enemyMove(Player& enemy);
};
// Example usage:
//Player player1("Alice", 100);
//player1.SetLocation(&startRoom); // Set the player's starting room