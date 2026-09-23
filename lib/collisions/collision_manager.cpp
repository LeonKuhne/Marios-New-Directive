#include "collision_manager.h"
#include "collider_body.h"

void CollisionManager::enterCollision(btPersistentManifold* const& manifold) {
  const std::function<void(CollisionEvent&)> callback = [](CollisionEvent& event) {
    auto hook = event.self->on_enter;
    if (hook) hook(event);
  };
  CollisionManager::onCollision(manifold, callback);
}

void CollisionManager::exitCollision(btPersistentManifold* const& manifold) {
  const std::function<void(CollisionEvent&)> callback = [](CollisionEvent& event) {
    auto hook = event.self->on_exit;
    if (hook) hook(event);
  };
  CollisionManager::onCollision(manifold, callback);
}

void CollisionManager::onCollision(btPersistentManifold* const& manifold, const std::function<void(CollisionEvent&)>& callback)
{
  ColliderBody* collidable_a = static_cast<ColliderBody*>(manifold->getBody0()->getUserPointer());
  ColliderBody* collidable_b = static_cast<ColliderBody*>(manifold->getBody1()->getUserPointer());
  if (collidable_a && collidable_b)
  {
    const btManifoldPoint& contact = manifold->getContactPoint(0);
    btVector3 normal = contact.m_normalWorldOnB;

    CollisionEvent event_a(collidable_a, collidable_b, manifold, CollisionEvent::Side::A);
    CollisionEvent event_b(collidable_b, collidable_a, manifold, CollisionEvent::Side::B);

    callback(event_a);
    callback(event_b);
  }
}
