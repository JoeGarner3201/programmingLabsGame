#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Item.h"
#pragma once

class Room {
private:
	std::string description;
	std::string roomName;
	std::map<std::string, Room*> exits;
	std::vector<Item> items;
public:
	Room(const std::string& name, const std::string& desc);
	void AddExit(const std::string& direction, Room* room); //Adds an exit to a room given direction and the room
	std::string GetName() const;
	std::string GetDescription() const;
	Room* GetExit(const std::string& direction) const; //Returns the room in the given direction
	const std::map<std::string, Room*>& GetExits() const; //Returns all the exits of the room
};