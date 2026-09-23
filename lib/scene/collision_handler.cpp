#include "collision_handler.h"
#include "lib/rooms/portal.h"
#include "lib/rooms/room.h"
#include "lib/rooms/room_manager.h"
#include "player.h"
#include <algorithm>
#include <utility>

CollisionHandler* CollisionHandler::instance = nullptr;

CollisionHandler::CollisionHandler(Player& player, RoomManager& room_manager)
  : player(player), room_manager(room_manager)
{
  instance = this;
  gContactStartedCallback = &CollisionHandler::contactStarted;
  gContactEndedCallback = &CollisionHandler::contactEnded;
}

void CollisionHandler::setActiveRoomsCallback(std::function<void(const std::vector<Room*>&)> callback)
{
  active_rooms_callback = std::move(callback);
}

void CollisionHandler::setCurrentRoom(Room& room)
{
  current_room = &room;
  rebuildActiveRooms();
}

void CollisionHandler::contactStarted(btPersistentManifold* const& manifold)
{
  if (instance)
    instance->onContactStarted(manifold);
}

void CollisionHandler::contactEnded(btPersistentManifold* const& manifold)
{
  if (instance)
    instance->onContactEnded(manifold);
}

void CollisionHandler::onContactStarted(btPersistentManifold* manifold)
{
  const btCollisionObject* body_a = manifold->getBody0();
  const btCollisionObject* body_b = manifold->getBody1();

  if (body_a != player.body && body_b != player.body)
    return;

  const btCollisionObject* other = body_a == player.body ? body_b : body_a;
  if (Portal* portal = findPortal(other))
  {
    glm::vec3 displacement = player.getPosition() - portal->center;
    portal_entries[portal] = {current_room, glm::dot(displacement, portal->normal())};
    rebuildActiveRooms();
  }

  Player* grounded_player = Player::isPlayerCollidingWithGround(body_a, body_b);
  if (!grounded_player)
    return;

  for (int contact_index = 0; contact_index < manifold->getNumContacts(); ++contact_index)
  {
    const btManifoldPoint& contact = manifold->getContactPoint(contact_index);
    if (contact.getDistance() > 0.05f)
      continue;

    btVector3 normal = contact.m_normalWorldOnB;
    if (body_b == player.body)
      normal = -normal;

    btVector3 player_position = player.body->getCenterOfMassPosition();
    btVector3 player_up = player_position.normalized();
    if (normal.dot(player_up) <= 0.5f)
      continue;

    grounded_player->isGrounded = true;
    btVector3 velocity = player.body->getLinearVelocity();
    velocity -= normal * velocity.dot(normal);
    player.body->setLinearVelocity(velocity);
    break;
  }
}

void CollisionHandler::onContactEnded(btPersistentManifold* manifold)
{
  const btCollisionObject* body_a = manifold->getBody0();
  const btCollisionObject* body_b = manifold->getBody1();

  if (Player* grounded_player = Player::isPlayerCollidingWithGround(body_a, body_b))
    grounded_player->isGrounded = false;

  if (body_a != player.body && body_b != player.body)
    return;

  const btCollisionObject* other = body_a == player.body ? body_b : body_a;
  if (Portal* portal = findPortal(other))
  {
    auto entry = portal_entries.find(portal);
    if (entry != portal_entries.end())
    {
      float exit_side = glm::dot(player.getPosition() - portal->center, portal->normal());
      constexpr float side_epsilon = 1e-4f;
      bool crossed = (entry->second.side < -side_epsilon && exit_side > side_epsilon)
        || (entry->second.side > side_epsilon && exit_side < -side_epsilon);
      Room& room_before = *entry->second.room_before;
      if (crossed)
        current_room = &portal->otherRoom(room_before);
      portal_entries.erase(entry);
      rebuildActiveRooms();
    }
  }
}

void CollisionHandler::setActiveRooms()
{
  if (active_rooms_callback)
    active_rooms_callback(active_rooms);
}

void CollisionHandler::rebuildActiveRooms()
{
  active_rooms.clear();
  if (current_room)
    active_rooms.emplace_back(current_room);

  for (const auto& [portal, entry] : portal_entries)
  {
    if (!std::ranges::contains(active_rooms, &portal->room_a))
      active_rooms.emplace_back(&portal->room_a);
    if (!std::ranges::contains(active_rooms, &portal->room_b))
      active_rooms.emplace_back(&portal->room_b);
  }
  setActiveRooms();
}

Portal* CollisionHandler::findPortal(const btCollisionObject* object)
{
  for (const RoomManager::PortalConnection& connection : room_manager.portals())
    if (connection.portal->shape->body == object)
      return connection.portal;
  return nullptr;
}
