#include "Room.h"
#include <iostream>
#pragma once
class Room;
class Area {
private:
	std::map<std::string, Room*> rooms;
public:
	void AddRoom(const std::string& name, Room* room);
	Room* GetRoom(const std::string& name);
	Room* GetRandomRoom();
	void ConnectRooms(const std::string& room1Name, const std::string&room2Name, const std::string& direction, const std::string& invertedDirection);
	void LoadMapFromFile(const std::string& filename);
};

