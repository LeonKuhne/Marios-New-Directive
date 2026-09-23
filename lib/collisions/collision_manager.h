#pragma once

#include "BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"
#include "lib/collisions/collision_event.h"
#include <functional>

class CollisionManager
{

public:
  CollisionManager()
  {
    gContactStartedCallback = &enterCollision;
    gContactEndedCallback = &exitCollision;
  }

  static void enterCollision(btPersistentManifold* const& manifold);
  static void exitCollision(btPersistentManifold* const& manifold);

private:
  static void onCollision(btPersistentManifold* const& manifold, const std::function<void(CollisionEvent&)>& callback);
};