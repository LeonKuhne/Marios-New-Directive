#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <btBulletDynamicsCommon.h>
#include "lib/shapes/shape_base.h"
#include "lib/util/vertex.h"
#include "lib/engine/config.h"

struct PlayerInfo
{
  glm::vec3 pos;
  glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 linear_velocity = glm::vec3(0.0f);
  glm::vec3 angular_velocity = glm::vec3(0.0f);
  float linear_damping = 0.0f;
  float angular_damping = 0.99f;
  float mass = 1.0f;
  glm::vec3 scale = glm::vec3(1.0f);
};

class Player : public ShapeBase
{
public:
  btRigidBody *body;
  bool isGrounded = false;
  bool isSprinting = false;
  glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

  Player(PlayerInfo info); 

  glm::vec3 getPosition() { return asGlmVec3(body->getWorldTransform().getOrigin()); }
  glm::vec3 getGravity() { return asGlmVec3(body->getGravity()); }
  void setSprinting(bool sprinting) { isSprinting = sprinting; }
  void move(const glm::vec2 &direction);
  void jump();
  ushort getType() override { return ShapeType::PLAYER; }

private:
  void setupGroundedListener();
};