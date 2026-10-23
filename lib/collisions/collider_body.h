#pragma once

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "lib/collisions/collision_event.h"
#include <functional>

class ColliderBody : public btRigidBody {
private:
  const uint8_t collision_group;
  void* owner = nullptr;

public:
  std::function<void(CollisionEvent&)> on_enter;
  std::function<void(CollisionEvent&)> on_exit;

  ColliderBody(uint8_t collision_group, btRigidBodyConstructionInfo& info) 
    : btRigidBody(info), collision_group(collision_group)
  {
    setUserPointer(this);
  }

  void setOwner(void* owner)
  {
    this->owner = owner;
  }

  template<typename T>
  T* getOwner()
  {
    return static_cast<T*>(owner);
  }

  bool isGroup(const uint8_t group) const
  {
    return collision_group == group;
  }
};