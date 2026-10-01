#include "room_manager.h"
#include <algorithm>
#include "lib/scene/scene.h"

Room& RoomManager::createRoom()
{
  owned_rooms.emplace_back(std::make_unique<Room>(ctx));
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

void RoomManager::setActive(Room* room)
{
  active_rooms.push_back(room);

  // create shape manager for active room
  bool active_room_exists = active_render_objects.contains(room);
  if (active_room_exists)
    active_render_objects.erase(room);
  std::vector<RenderObject>* render_objects = active_render_objects.emplace(room, new std::vector<RenderObject>).first->second;
  
  // insert room's shapes into the shape manager
  room->collect_visible_render_objects(render_objects);
}

void RoomManager::updateVisibility()
{
  for (const std::unique_ptr<Room>& room : owned_rooms)
    room->updateVisibility();
}

void RoomManager::render(Scene& scene, SDL_GPURenderPass *pass)
{
  for (auto [room, objects] : active_render_objects)
    for (RenderObject& obj : *objects)
      scene.pbr_pipeline.render(scene, obj);
}

size_t RoomManager::ConnectionHash::operator()(const ConnectionKey& key) const
{
  size_t first_hash = std::hash<Room*>{}(key.first);
  size_t second_hash = std::hash<Room*>{}(key.second);
  return first_hash ^ (second_hash + 0x9e3779b9u + (first_hash << 6) + (first_hash >> 2));
}