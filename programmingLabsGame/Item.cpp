#include "Item.h"
#include "Character.h"
#include "Room.h"
#include "GameItems.h"
#include "Area.h"
#include <iostream>
#include <string>

Item::Item(const std::string& name, const std::string& desc) {
	this->name = name;
	this->description = desc;
}

int Item::GetDamage() const {
	return 0; //Return 0 if the item is not a weapon
}

void Item::Interact(Player& player, Player& enemy, Room* currentRoom, Area& gameWorld) {//All item interactions are handled here

	std::string roomName = currentRoom->GetName();
	std::cout << "You use the " << name << ".\n";

		if (name == "Healthpack") {//Heals the player, has no room requirement so you can use it anywhere as long as the player has below max health
			if (player.GetHealth() < 100) {	
				player.TakeDamage(-50);
				if (player.GetHealth() > 100) { //Makes sure you don't go over 100 health by setting you back to 100 if you do
					player.SetHealth(100);
				}
				player.RemoveItem(player.GetItem("Healthpack"));//remove healthpack from inventory
			} else {
				std::cout << "You are already at full health.\n";//print message if player is at full health and can't use the item
			}
		}

		else if (name == "Branch" and roomName == "Hallway") {//If player uses the branch in the hallway, it turns into a torch (using the candle)
			std::cout << "Using the candle on the wall, you set the branch on fire, turning it into a makeshift torch.\n";
			Item* torch = new Item("Torch", "A torch that can light up dark areas.");
			player.AddItem(torch);
			player.RemoveItem(player.GetItem("Branch"));//Remove branch afterwards
		}

		else if (name == "Torch" and roomName == "Cavern") {//If the player uses the torch in the cave it lights up and you find a sword
			std::cout << "The room is now illuminated, you find a sword on the ground and pick it up.\n";
			Item* sword = new Weapon("Sword", "A sharp sword that can help defeat monsters and cut vines.", 40, 6);//Define sword object
			player.AddItem(sword);//Add sword
		}

		else if (roomName == "Forest") {
			if (name == "Sword" or name == "Axe") {//If you have a sword or axe you can cut vines within the forest room
				std::cout << "You cut down some vines that could be useful later on.\n";
				if (player.GetItem("Vines") == nullptr) {//If you already have vines then you can't get more
					Item* vines = new Item("Vines", "Thick vines that can be used as rope.");
					player.AddItem(vines);//add vines 
				}
			}
		}

		else if (name == "Sword" and roomName == "Treasure Room") {//If you have a sword in the treasure room you can open the chest
			if (player.GetItem("Key") == nullptr) {//Only works if you don't have the key, this acts as a check to see if players have already opened the chest or not
				std::cout << "You throw the weapon at the trap, and the hilt blocks the mechanism, letting you pass.\n";
				std::cout << "Within the chest you find a key.\n";
				player.RemoveItem(player.GetItem("Sword"));//remove sword fron inventory
				Item* key = new Item("Key", "A key that can be used to open a door.");
				player.AddItem(key);//add key
			}
		}

		else if (name == "Vines" and roomName == "River") {//If you have vines in the river room you can make a rope to cross the river
			std::cout << "You make a rope that will let you cross the river.\n";
			gameWorld.ConnectRooms("River", "Final Room", "north", "south");//Connect the two rooms
			player.RemoveItem(player.GetItem("Vines"));//Remove vines from inventory
		}

		else if (name == "Key" and roomName == "Final Room") {//If you have the key in the final room you can unlock the door and win the game
			std::cout << "You unlock the door and escape the dungeon.\n";
			std::cout << "You win!\n";
			exit(0);//Exit game
		}
		else {
			std::cout << "Nothing happens.\n";//print message if nothing happens
		}
}

void Weapon::Interact(Player& player, Player& enemy, Room* currentRoom, Area& gameWorld) {
	if (enemy.GetLocation()->GetName() == player.GetLocation()->GetName()) {
		std::cout << "Attack enemy or use the item? (attack/use) \n";
		std::string choice;
		std::cin >> choice;
		choice[0] = toupper(choice[0]);
		if (choice == "Attack") {
			std::cout << "Throw weapon? (yes/no) \n"; //Throwing a weapon will deal triple damage if it hits, but the weapon will break
			std::string attackType;
			std::cin >> attackType;
			attackType[0] = toupper(attackType[0]);
			if (attackType == "Yes") {
				std::cout << "You throw the weapon at the enemy.\n";
				srand(time(0));
				int hitChance = rand() % 100 + 1;
				if (hitChance <= 90) { //90% chance to hit the enemy
					std::cout << "Successful hit! \n";
					enemy.TakeDamage(damage * 3);
				}
				else {
					std::cout << "You missed.\n";
				}
				player.RemoveItem(this);//Remove weapon regardless of if the throww was successful or not
			}
			else {
				std::cout << "You attack the enemy with the weapon.\n";
				enemy.TakeDamage(damage);
				durability--;
				if (durability <= 0) {
					std::cout << "The weapon broke!\n";
					player.RemoveItem(this);//Remove weapon if it's broken
				}
			}
		}
		else {
			Item::Interact(player, enemy, currentRoom, gameWorld); //If the player chooses to not attack, use the original item interaction
		}
	} else {
		Item::Interact(player, enemy, currentRoom, gameWorld); //If the enemy is not in the same room, use the original item interaction since you can't attack
	}	
}

int Weapon::EnemyAttack(Player& player, Player& enemy) {
	player.TakeDamage(damage);
	durability--;
	if (durability <= 0) {
		durability = 2;
		damage+=10; //Increase damage for next time and reset durability
		return 0;
	} else {
		return 1;
	}
}

std::string Item::GetName() const {
	return name;
}

std::string Item::GetDescription() const {
	return description;
}

int Weapon::GetDamage() const {
	return damage;
}

int Weapon::GetDurability() const {
	return durability;
}

void Weapon::SetDurability(int newDurability) {
	durability = newDurability;
}

