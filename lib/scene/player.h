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
  float linear_damping = 0.0f;
  float angular_damping = 1.0f;
  float mass = 70.0f;
};

class Player : public ShapeBase
{
public:
  btRigidBody *body;
  bool isGrounded = false;
  bool isSprinting = false;
  glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  float eye_height = Config::PlayerSettings::eye_height;
  Uint64 last_jump_time = 0;
private:
  glm::vec2 move_dir = glm::vec2(0.0f, 0.0f);

public:
  Player(PlayerInfo info); 

  // hooks
  void tick();

  // getters
  glm::vec3 getPosition() { return asGlmVec3(body->getWorldTransform().getOrigin()); }
  glm::vec3 getGravity() { return asGlmVec3(body->getGravity()); }
  ushort getType() override { return ShapeType::PLAYER; }

  // setters
  void setSprinting(bool sprinting) { isSprinting = sprinting; }
  void move(const glm::vec2 &direction);
  void jump();

private:
  void applyMove();
  static void setupGroundedListener();
  inline static bool init = []() { setupGroundedListener(); return true; }();
public:
  static Player *isPlayerCollidingWithGround(const btCollisionObject *a, const btCollisionObject *b);
  static Player *isPlayerGravitonCollision(const btCollisionObject *a, const btCollisionObject *b);
};