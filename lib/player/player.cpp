#include "player.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>

Player::Player(Info info)
{
  btVector3 localInertia;
  btScalar capsule_height = Config::PlayerSettings::height - 2.0f * Config::PlayerSettings::radius;
  btCollisionShape *capsule_collider = new btCapsuleShape(Config::PlayerSettings::radius, capsule_height);
  capsule_collider->calculateLocalInertia(info.mass, localInertia);

  auto *player_collider = new btCompoundShape();
  btTransform offset;
  offset.setIdentity();
  offset.setOrigin(btVector3(0.0f, Config::PlayerSettings::height * 0.5f, 0.0f));
  player_collider->addChildShape(offset, capsule_collider);

  btTransform transform;
  transform.setIdentity();
  transform.setRotation(asBtQuaternion(info.rotation));
  transform.setOrigin(asBtVector3(info.pos - glm::vec3(0.0f, Config::PlayerSettings::height * 0.5f, 0.0f)));

  btDefaultMotionState *motionState = new btDefaultMotionState(transform);
  btRigidBody::btRigidBodyConstructionInfo bodyInfo(info.mass, motionState, player_collider, localInertia);
  body = new PlayerBody(*this, Config::Colliders::PLAYER, bodyInfo);
  body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
  body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
  body->setDamping(info.linear_damping, info.angular_damping);
  body->setFriction(1.0f);
  body->setRollingFriction(0.0f);
  body->setRestitution(0.0f);
  body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}

void Player::tick() {
  applyMove();
}

void Player::move(const glm::vec2 &direction)
{
  move_dir += direction;
}

void Player::applyMove() {
  if (move_dir.x == 0.0f && move_dir.y == 0.0f)
    return;

  btVector3 velocity = body->getLinearVelocity();
  glm::vec2 velocity_xy = glm::vec2(velocity.getX(), velocity.getZ());

  glm::vec3 forward_dir = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
  glm::vec3 right_dir = glm::vec3(forward_dir.z, 0.0f, -forward_dir.x);

  // add delta
  glm::vec3 delta = forward_dir * move_dir.x + right_dir * move_dir.y;
  move_dir = glm::vec2(0.0f, 0.0f);

  float move_accel = isSprinting ? Config::PlayerSettings::sprint_accel : Config::PlayerSettings::walk_accel;
  if (!isGrounded)
    move_accel *= Config::PlayerSettings::air_movement_factor; 

  // limit move speed
  if (glm::length(velocity_xy) > Config::PlayerSettings::max_speed)
    return;

  velocity_xy += glm::normalize(glm::vec2(delta.x, delta.z)) * move_accel;

  // update velocity
  velocity.setX(velocity_xy.x);
  velocity.setZ(velocity_xy.y);
  body->activate(true);
  body->setLinearVelocity(velocity);
}


void Player::jump()
{
  Uint64 current_time = SDL_GetTicks();
  if (!isGrounded || current_time - last_jump_time < Config::PlayerSettings::jump_cooldown)
    return;
  last_jump_time = current_time;

  glm::vec3 up_axis = glm::dot(up, up) > 1e-6f
                          ? glm::normalize(up)
                          : glm::vec3(0.0f, 1.0f, 0.0f);
  body->activate(true);
  body->applyCentralImpulse(asBtVector3(up_axis * Config::PlayerSettings::jump_strength * body->getMass()));
}