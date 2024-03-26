#include "Area.h"
#include "Room.h"
#include <iostream>
#include <fstream>
#include <sstream>

Room* Area::GetRoom(const std::string& name) {
	if (rooms.find(name) != rooms.end()) { //Loops through the map of rooms to try and find the room with the given name
		return rooms[name];
	}
	else {
		std::cout << "Error: Room " << name << " not found.\n";
		return nullptr;
	}
}

Room* Area::GetRandomRoom() {
	// Create a list of all rooms
	std::vector<Room*> allRooms;
	for (const auto& pair : rooms) {
		allRooms.push_back(pair.second);
	}

	// Remove the starting room and the final room from the list since we don't want the enemy to spawn there
	allRooms.erase(std::remove(allRooms.begin(), allRooms.end(), GetRoom("Starting Room")), allRooms.end());
	allRooms.erase(std::remove(allRooms.begin(), allRooms.end(), GetRoom("Final Room")), allRooms.end());

	// Randomly select a room from the list
	srand(time(0));
	int randomIndex = rand() % allRooms.size();
	return allRooms[randomIndex];
}

void Area::AddRoom(const std::string& name, Room* room) {
	rooms[name] = room;
}

void Area::LoadMapFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::istringstream ss(line);
			std::string firstRoom, secondRoom, direction, invertedDirection;
			std::getline(ss, firstRoom, '|');
			std::getline(ss, secondRoom, '|');
			std::getline(ss, direction, '|');
			std::getline(ss, invertedDirection, '|');
			Area::ConnectRooms(firstRoom, secondRoom, direction, invertedDirection);
		}
	}
	else {
		std::cout << "Error: Unable to open file " << filename << "\n";
	}
}
void Area::ConnectRooms(const std::string& firstRoom, const std::string& secondRoom,const std::string& direction,const std::string& invertedDirection) {
	Room* room1 = GetRoom(firstRoom);
	Room* room2 = GetRoom(secondRoom);
	if (room1 != nullptr && room2 != nullptr) {
		room1->AddExit(direction, room2);
		room2->AddExit(invertedDirection, room1);
		}
	else {
		std::cout << "Error: Unable to connect rooms " << firstRoom << " and " << secondRoom << "\n";
	}
}
