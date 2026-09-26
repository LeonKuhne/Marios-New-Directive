#include "room_manager.h"
#include <algorithm>

Room& RoomManager::createRoom()
{
  owned_rooms.emplace_back(std::make_unique<Room>(context));
  room_pointers.emplace_back(owned_rooms.back().get());
  return *owned_rooms.back();
}

void RoomManager::connect(Room& first, ShapeData& shape_data, Room& second)
{
  Room* lower = std::min(&first, &second);
  Room* upper = std::max(&first, &second);
  if (!connections.emplace(lower, upper).second)
    return;

  // todo create a portal class that exetends trigger
  // construct portal gameobject from shape data
  const Trigger::Info info{
    .solid = {
      .shape = shape_data,
      .density = shape_data.density
    }
  };
  Trigger* trigger = new Trigger(info);
  first.shapes.add(trigger);

  // construct portal object
  owned_portals.emplace_back(std::make_unique<Portal>(trigger, first, second));
  Portal* portal = owned_portals.back().get();
  first.addPortal(portal);
  second.addPortal(portal);

  portal_connections.push_back({
    .room_a=&first,
    .room_b=&second,
    .portal=portal,
  });
}

void RoomManager::setActive(Room& room)
{
  active_rooms.push_back(&room);
}

void RoomManager::updateVisibility()
{
  for (const std::unique_ptr<Room>& room : owned_rooms)
    room->updateVisibility();
}

void RoomManager::render(Scene& scene, SDL_GPURenderPass *pass)
{
  for (Room* room : active_rooms)
    room->render(scene, pass);
}

size_t RoomManager::ConnectionHash::operator()(const ConnectionKey& key) const
{
  size_t first_hash = std::hash<Room*>{}(key.first);
  size_t second_hash = std::hash<Room*>{}(key.second);
  return first_hash ^ (second_hash + 0x9e3779b9u + (first_hash << 6) + (first_hash >> 2));
}