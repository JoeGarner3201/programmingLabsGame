#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include "Item.h"
#include "Room.h"
#include "Character.h"
#include "Area.h"

// Include the Room and Item classes (assuming they are defined in separate
//files, which they should be!)
int main() {
	// Create an instance of the Area class
	Area gameWorld;
	int respawnTimer = 0;

	Room* hallway = new Room("Hallway", "You are in a long hallway.");
	gameWorld.AddRoom("Hallway", hallway);
	Room* startRoom = new Room("Starting Room", "You are in an empty room.");
	gameWorld.AddRoom("Starting Room", startRoom);
	Room* armoury = new Room("Armoury", "You are in a room full of weapons, however they are all rusted and worn.");
	gameWorld.AddRoom("Armoury", armoury);
	Room* cavern = new Room("Cavern", "You are in a dark cave.");
	gameWorld.AddRoom("Cavern", cavern);
	Room* treasureRoom = new Room("Treasure Room", "You are in a room with a treasure chest in the centre.");
	gameWorld.AddRoom("Treasure Room", treasureRoom);
	Room* riverRoom = new Room("River", "A river flows through the room, preventing you from getting across.");
	gameWorld.AddRoom("River", riverRoom);
	Room* forestRoom = new Room("Forest", "You are in a dense forest.");
	gameWorld.AddRoom("Forest", forestRoom);
	Room* finalRoom = new Room("Final Room", "You are at the exit door. The door requires a key.");
	gameWorld.AddRoom("Final Room", finalRoom);

	// Load the game map from a text file
	gameWorld.LoadMapFromFile("game_map.txt");
	// Create a Player
	Player player("Alice", 100);
	Player enemy("Hunter", 100);

	// Set the player's starting room (you can modify this room name)
	Room* currentRoom = gameWorld.GetRoom("Starting Room"); //Edit this to change where you start
	player.SetLocation(currentRoom);
	//randomize a location for the enemy
	Room* enemyRoom = gameWorld.GetRandomRoom();
	enemy.SetLocation(enemyRoom);
	std::cout << "The hunter is in the " << enemy.GetLocation()->GetName() << " area" << std::endl;
	Weapon* dagger = new Weapon("Dagger", "A sharp dagger that deals decent damage.", 20, 3);
	enemy.AddItem(dagger);

	// Game loop
	while (true) {
		if (enemy.GetHealth() <= 0) {
			std::cout << "The hunter is defeated for now, but will respawn again.\n";
			enemy.SetLocation(gameWorld.GetRoom("Armoury")); //enemy respawns in the armoury but cannot move until the player has moved 3 times
			enemy.SetHealth(100);
			respawnTimer = 3;
		}

		//Enemy attacks player if they are in the same room and enemy has health above 0
		if (enemy.GetLocation()->GetName() == player.GetLocation()->GetName()) {
			std::cout << "The enemy attacks you!\n";
			if (dagger->EnemyAttack(player, enemy) == 0) {
				std::cout << "Enemy weapon has broken, they retreat to the Armoury to sharpen their weapon.\n";
				enemy.SetLocation(gameWorld.GetRoom("Armoury"));
			}
		}

		//Check if player is dead, if so, break out of the loop
		if (player.GetHealth() <= 0) {
			break;
		}
		std::cout << "Health: " << player.GetHealth() << std::endl;
		std::cout << "Current Location: " << player.GetLocation()->GetDescription() << std::endl;
		std::cout << "Items in your inventory:" << std::endl;
		std::cout << player.printInventory() << std::endl;
		std::cout << "Exits available: ";
		for (const auto& exit : player.GetLocation()->GetExits()) {
			std::cout << exit.first << " ";
		}
		std::cout << "| ";
		std::cout << "Options: ";
		std::cout << "1. Look around | ";
		std::cout << "2. Interact with an item | ";
		std::cout << "3. Move to another room | ";
		std::cout << "4. Quit" << std::endl;
		int choice;
		std::cin >> choice;
		if (std::cin.fail()) {
			std::cin.clear(); // clear the fail state
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the rest of the bad input
			std::cout << "Invalid input. Please enter a number.\n";
			continue;
		}
		if (choice == 1) {
			// Player looks around (This is modified to interact with the room if applicable)
			std::cout << "You look around the room." << std::endl;
			player.RoomInteract();
		}
		else if (choice == 2) {
			// Player interacts with an item
			std::cout << "Enter the name of the item you want to interact with: ";
			std::string itemName;
			std::cin >> itemName;
			itemName[0] = toupper(itemName[0]);
			Item* currentItem = player.GetItem(itemName);
			if (currentItem != nullptr) {
				currentItem->Interact(player, enemy, currentRoom, gameWorld);
				enemy.enemyMove(enemy);	
			}
			else {
				std::cout << "You don't have that item!\n";
			}
		}
		else if (choice == 3) {
			// Player moves to another room
			std::cout << "Enter the direction: ";
			std::string direction;
			std::cin >> direction;
			Room* nextRoom = player.GetLocation()->GetExit(direction);
			if (nextRoom != nullptr) {
				player.SetLocation(nextRoom);
				std::cout << "You move to the next room." << std::endl;
				currentRoom = nextRoom;
				//if successful room change then move enemy as well
				//Enemy chooses a random exit to move to, but 25% of the time they will not move
				//When respawnTimer is active, enemy does not move and the timer decrements
				if (respawnTimer > 0) {
					respawnTimer--;
				}
				if (respawnTimer == 0) {
					enemy.enemyMove(enemy);
				}
				else {
					std::cout << "You can't go that way." << std::endl;
				}
			}
		}
		else if (choice == 4) {
			// Quit the game
			std::cout << "Goodbye!" << std::endl;
			break;
		}
	}
	return 0;
}