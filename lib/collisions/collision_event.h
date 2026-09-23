#pragma once

#include "BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"
#include <cstdint>

class ColliderBody;

class CollisionEvent
{
public:
  enum Side : uint8_t {A, B};
  ColliderBody* self;
  ColliderBody* other;
  btPersistentManifold* const& manifold;

private:
  Side side;

public:
  CollisionEvent(ColliderBody* self, ColliderBody* other, btPersistentManifold* const& manifold, Side side) 
    : self(self), other(other), manifold(manifold), side(side) {}

  btVector3 getNormal() const { 
    btVector3 normal = manifold->getContactPoint(0).m_normalWorldOnB; 
    if (side == B)
      return -normal;
    return normal;
  }
};