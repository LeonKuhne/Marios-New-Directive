#include "player_body.h"
#include "player.h"

void PlayerBody::onEnter(CollisionEvent& event)
{
  if (!isPlayerCollidingWithSolid(event.other))
    return;

  for (int contact_index = 0; contact_index < event.manifold->getNumContacts(); ++contact_index)
  {
    const btManifoldPoint& contact = event.manifold->getContactPoint(contact_index);
    if (contact.getDistance() > 0.05f)
      continue;

    btVector3 player_position = getCenterOfMassPosition();
    btVector3 player_up = player_position.normalized();
    btVector3 normal = event.getNormal();
    if (normal.dot(player_up) <= 0.5f)
      continue;

    player.isGrounded = true;
    btVector3 velocity = getLinearVelocity();
    velocity -= normal * velocity.dot(normal);
    setLinearVelocity(velocity);
    break;
  }
}
  
void PlayerBody::onExit(CollisionEvent& event)
{
  const btCollisionObject* body_a = event.manifold->getBody0();
  const btCollisionObject* body_b = event.manifold->getBody1();

  if (isPlayerCollidingWithSolid(event.other))
    player.isGrounded = false;

  /* this is all portal code
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
  */
}

bool PlayerBody::isPlayerCollidingWithSolid(const ColliderBody *other)
{
    return other->isGroup(Config::Colliders::SURFACE);
}