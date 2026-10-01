#pragma once

#include "room.h"
#include <cwchar>
#include <map>
#include <memory>
#include <unordered_set>
#include <vector>

class RoomManager
{
public:
  struct PortalConnection
  {
    Room* room_a;
    Room* room_b;
    Portal* portal;
  };

private:
  struct ConnectionKey
  {
    Room* first;
    Room* second;
    bool operator==(const ConnectionKey&) const = default;
  };

  struct ConnectionHash
  {
    size_t operator()(const ConnectionKey& key) const;
  };

private:
  Context& ctx;
  std::vector<std::unique_ptr<Room>> owned_rooms;
  std::vector<Room*> room_pointers;
  std::vector<std::unique_ptr<Portal>> owned_portals;
  std::unordered_set<ConnectionKey, ConnectionHash> connections;
  std::vector<PortalConnection> portal_connections;
  std::function<void(const std::vector<Room*>&)> active_rooms_changed;
  std::map<Room*, std::vector<RenderObject>*> active_render_objects;

public:
  std::vector<Room*> active_rooms;

  explicit RoomManager(Context& ctx) : ctx(ctx) {}

  Room& createRoom();
  void connect(Room& first, ShapeData& portal, Room& second);
  void updateVisibility();
  void render(Scene& scene, SDL_GPURenderPass *pass);
  void setActive(Room* room);
  Room* find(int index) const { return room_pointers[index]; }

  const std::vector<Room*>& rooms() const { return room_pointers; }
  const std::vector<PortalConnection>& portals() const { return portal_connections; }
};
