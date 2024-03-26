#include "Character.h"
#include "GameItems.h"
#include <iostream>
#include <string>

Character::Character(const std::string& name, int health) {
	this->name = name;
	this->health = health;
}
void Character::TakeDamage(int damage) {
	health -= damage;
	if (damage < 0) {
		std::cout << name << " heals for " << -damage << " health.\n"; //Negative damage = healing the player (e.g by using the medkit)
	}
	else {
		std::cout << name << " takes " << damage << " damage.\n";
	}
	
	if (health <= 0) {
		std::cout << name << " has died. Game Over!\n";
	}
}	

int Character::GetHealth() const {
	return health;
}

void Character::SetHealth(int newHealth) {
	health = newHealth;
}

void Character::AddItem(Item* item) {
	inventory.push_back(item);
}

void Character::RemoveItem(Item* item) {
	for (int i = 0; i < inventory.size(); i++) {
		if (inventory[i]->GetName() == item->GetName()) {
			delete inventory[i]; // Delete the item from memory
			inventory.erase(inventory.begin() + i);
			break;
		}
	}
}

std::string Character::printInventory() {
	std::string inventoryList = "";
	for (int i = 0; i < inventory.size(); i++) {
		inventoryList += inventory[i]->GetName() + "\n"; // Use -> instead of .
	}
	return inventoryList;
}

Item* Character::GetItem(const std::string& itemName) {
	for (int i = 0; i < inventory.size(); i++) {
		if (inventory[i]->GetName() == itemName) {
			return inventory[i]; // Return the Item pointer, not a reference to the Item
		}
	}
	return nullptr;
}

Player::Player(const std::string& name, int health) : Character(name, health), location(nullptr) {
	//For this constructor the body is empty because all the work is done in the initializer list above
}

void Player::SetLocation(Room* newLocation) {
	location = newLocation;
}
Room* Player::GetLocation() {
	return location;
}

void Player::enemyMove(Player& enemy) {
	srand(time(0));//Seed random number generator so it's different each time
	int moveChance = rand() % 4;//roll 25% chance to not move
	if (moveChance != 0) {
		std::vector<std::string> exits;
		for (const auto& exit : enemy.GetLocation()->GetExits()) {//Get all the exits from the current room
			exits.push_back(exit.first); // Add the exit to the vector
		}
		if (!exits.empty()) {
			int randomIndex = rand() % exits.size(); // Randomly select an index based on the size of the vector
			std::string randomExit = exits[randomIndex]; // Get the exit from the vector using the random index
			Room* nextRoom = enemy.GetLocation()->GetExit(randomExit); // Get the room pointer from the exit
			if (nextRoom != nullptr) { //If the room pointer is not null
				enemy.SetLocation(nextRoom); //Set the enemy's location to the new room
				std::cout << "The hunter moves to the " << enemy.GetLocation()->GetName() << " area" << std::endl; //Output message so the player knows where the enemy moved
			}
		}
	}
}

void Player::RoomInteract() {
	//Get player room location and then select the right case for the interaction
	Room* currentRoom = GetLocation();
	std::string roomName = currentRoom->GetName();
	
	if (roomName == "Cavern") {
		if (GetItem("Torch") == nullptr) {
			std::cout << "It's too dark to see. Suddenly, a stalactite falls from the roof!\n";
			srand(time(0)); //seeds the rng so it's different each time
			int damage = rand() % 21 + 30; //randomizes damage taken, will be in the range 30-50
			TakeDamage(damage);
		}
	} else if (roomName == "Hallway") {
		std::cout << "There is a candle fixed to the wall. You can't take it, but the flame might be useful.\n";
	} else if (roomName == "Armoury") {
		if (GetItem("Axe") == nullptr) {
			std::cout << "There's a worn and rusted axe on the ground, and you pick it up.\n";
			Item* axe = new Weapon("Axe", "A rusty axe that isn't very strong, but it's better than nothing", 25, 3);
			AddItem(axe);
		}
	} else if (roomName == "Starting Room") {
		if (GetItem("Healthpack") == nullptr) {
			std::cout << "You find a first aid kit on the ground and pick it up.\n";
			Item* healthpack = new Item("Healthpack", "Heals 50HP, single use.");
			AddItem(healthpack);
		}
	} else if (roomName == "River") {
		std::cout << "You can see a door on the other side of the river, but it's too deep and unfortunately you don't know how to swim.\n";
	} else if (roomName == "Treasure Room") {
		if (GetItem("Key") == nullptr) {
			std::cout << "A trap activates and shoots an arrow at you!\n";
			srand(time(0));
			int damage = rand() % 11 + 20;
			TakeDamage(damage);
		}
	} else if (roomName == "Forest") {
		std::cout << "There are some vines hanging down, but you can't cut them with just your bare hands.\n";
		if (GetItem("Branch") == nullptr) {
			std::cout << "You pick up some of the branches on the floor. \n";	
			Item* branch = new Item("Branch", "A sturdy branch that can be used as a torch.");
			AddItem(branch);
		}
	}
}
