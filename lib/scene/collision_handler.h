#pragma once

#include <btBulletDynamicsCommon.h>
#include <functional>
#include <unordered_map>
#include <vector>
class Player;
class Portal;
class Room;
class RoomManager;

class CollisionHandler
{
public:
  CollisionHandler(Player& player, RoomManager& room_manager);
  void setActiveRoomsCallback(std::function<void(const std::vector<Room*>&)> callback);
  void setCurrentRoom(Room& room);

private:
  Player& player;
  RoomManager& room_manager;
  std::function<void(const std::vector<Room*>&)> active_rooms_callback;
  Room* current_room = nullptr;
  std::vector<Room*> active_rooms;
  struct PortalEntry
  {
    Room* room_before;
    float side;
  };
  std::unordered_map<Portal*, PortalEntry> portal_entries;

  static CollisionHandler* instance;

  void onContactStarted(btPersistentManifold* manifold);
  void onContactEnded(btPersistentManifold* manifold);
  void setActiveRooms();
  void rebuildActiveRooms();
  Portal* findPortal(const btCollisionObject* object);

  static void contactStarted(btPersistentManifold* const& manifold);
  static void contactEnded(btPersistentManifold* const& manifold);
};
