#pragma once

#include "lib/rooms/room.h"
#include <map>
#include "cell.h"

class RenderMap
{
  using Rooms = std::map<uint32_t, Room*>;

public:
  static void renderAscii(const Rooms& rooms,
                          const std::vector<Room*>& active_rooms);
  static bool renderPng(const Rooms& rooms,
                        const std::vector<Room*>& active_rooms,
                        const char* path = "map.png");

private:
  static std::string cellSymbol(
    const RenderMap::Rooms& rooms,
    const std::vector<Room*>& active_rooms,
    const Cell& cell,
    const std::set<const Room*>& direct_neighbors, size_t& visible_count,
    size_t& missing_direct_count);
};
