#pragma once

#include "lib/rooms/room.h"
#include <map>

using Cell = std::pair<int, int>;

class VisibilityMap
{
public:
  static void renderAscii(const std::map<const Cell, Room*>& rooms,
                          const std::vector<Room*>& active_rooms);
  static bool renderPng(const std::map<const Cell, Room*>& rooms,
                        const std::vector<Room*>& active_rooms,
                        const char* path = "map.png");
};
