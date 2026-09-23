#pragma once

#include "room.h"
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

  explicit RoomManager(Context& context) : context(context) {}

  Room& createRoom();
  void connect(Room& first, ShapeData& portal, Room& second);
  void updateVisibility();
  void render(Scene& scene, SDL_GPURenderPass *pass);

  const std::vector<Room*>& rooms() const { return room_pointers; }
  const std::vector<PortalConnection>& portals() const { return portal_connections; }

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

  Context& context;
  std::vector<std::unique_ptr<Room>> owned_rooms;
  std::vector<Room*> room_pointers;
  std::vector<std::unique_ptr<Portal>> owned_portals;
  std::unordered_set<ConnectionKey, ConnectionHash> connections;
  std::vector<PortalConnection> portal_connections;
  std::vector<Room*> active_rooms;
  std::function<void(const std::vector<Room*>&)> active_rooms_changed;
};
