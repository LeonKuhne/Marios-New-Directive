#pragma once

#include "lib/rooms/room.h"
#include <map>

using Cell = std::pair<int, int>;

class VisibilityMap
{
public:
  static void renderAscii(const std::map<const Cell, Room&>& rooms, Room& active_room);
  static bool renderPng(const std::map<const Cell, Room&>& rooms, Room& active_room,
                        const char* path = "map.png");
};
