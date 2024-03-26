#include "room.h"
#include "item.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>

Room::Room(const std::string& name, const std::string& desc) {
	this->roomName = name;
	this->description = desc;
}	

void Room::AddExit(const std::string& direction, Room* room) {
	exits[direction] = room;
}

std::string Room::GetDescription() const {
	return description;
}
std::string Room::GetName() const {
	return roomName;
}

Room* Room::GetExit(const std::string& direction) const {
	if (exits.find(direction) != exits.end()) {
		return exits.at(direction);
	}
	else {
		return nullptr;
	}
}

const std::map<std::string, Room*>& Room::GetExits() const {
	return exits;
}