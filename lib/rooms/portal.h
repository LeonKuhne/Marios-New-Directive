#pragma once

#include <vector>

class Room;

class Portal {
private:
  std::vector<Room*> connected_rooms;
  std::vector<Room*> visible_rooms;
};
