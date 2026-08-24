#include "player.h"

Player::Player(PlayerInfo info)
{
  btVector3 localInertia;
  btCollisionShape *collider = new btSphereShape(1.0f);
  collider->setLocalScaling(asBtVector3(info.scale));
  collider->calculateLocalInertia(info.mass, localInertia);

  btTransform transform;
  transform.setIdentity();
  transform.setRotation(asBtQuaternion(info.rotation));
  transform.setOrigin(asBtVector3(info.pos));

  btDefaultMotionState *motionState = new btDefaultMotionState(transform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(info.mass, motionState, collider, localInertia);
  body = new btRigidBody(rbInfo);
  body->setLinearVelocity(asBtVector3(info.linear_velocity));
  body->setAngularVelocity(asBtVector3(info.angular_velocity));
  body->setDamping(info.linear_damping, info.angular_damping);
  body->setFriction(Config::player_ground_friction);
  body->setRollingFriction(Config::player_ground_rolling_friction);
  body->setUserPointer(this);
  body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

  setupGroundedListener();
}

static Player *isPlayerGravitonCollision(const btCollisionObject *a, const btCollisionObject *b)
{
  if (!a->getUserPointer() || !b->getUserPointer())
    return nullptr;

  uint type_a = static_cast<ShapeBase *>(a->getUserPointer())->getType();
  uint type_b = static_cast<ShapeBase *>(b->getUserPointer())->getType();
  if (type_a == ShapeType::GRAVITON && type_b == ShapeType::PLAYER) {
    return static_cast<Player *>(b->getUserPointer());
  } else if (type_a == ShapeType::PLAYER && type_b == ShapeType::GRAVITON) {
    return static_cast<Player *>(a->getUserPointer());
  }

  return nullptr;
}

void Player::setupGroundedListener()
{
  gContactAddedCallback = [](btManifoldPoint &cp,
                             const btCollisionObjectWrapper *a, int partId0, int index0,
                             const btCollisionObjectWrapper *b, int partId1, int index1)
  {
    static Player *player = isPlayerGravitonCollision(a->getCollisionObject(), b->getCollisionObject());
    if (player)
      player->isGrounded = true;
    return true;
  };

  gContactEndedCallback = [](btPersistentManifold *const &manifold)
  {
    static Player *player = isPlayerGravitonCollision(manifold->getBody0(), manifold->getBody1());
    if (player)
      player->isGrounded = false;
  };
}

void Player::move(const glm::vec2 &direction)
{
  if (!isGrounded)
    return;

  body->activate(true);
  glm::vec3 up_axis = glm::dot(up, up) > 1e-6f
                          ? glm::normalize(up)
                          : glm::vec3(0.0f, 1.0f, 0.0f);

  glm::vec3 forward_axis = forward - glm::dot(forward, up_axis) * up_axis;
  if (glm::dot(forward_axis, forward_axis) <= 1e-6f)
    forward_axis = glm::vec3(0.0f, 0.0f, -1.0f);
  else
    forward_axis = glm::normalize(forward_axis);

  glm::vec3 right_axis = glm::cross(forward_axis, up_axis);
  if (glm::dot(right_axis, right_axis) <= 1e-6f)
    right_axis = glm::vec3(1.0f, 0.0f, 0.0f);
  else
    right_axis = glm::normalize(right_axis);

  glm::vec3 delta = forward_axis * direction.x + right_axis * direction.y;
  float move_speed = isSprinting ? Config::player_sprint_speed : Config::player_speed;
  // accelerate in the direction of movement
  body->applyCentralForce(asBtVector3(delta * move_speed * body->getMass()));
}

void Player::jump()
{
  if (!isGrounded)
    return;

  body->activate(true);
  glm::vec3 up_axis = glm::dot(up, up) > 1e-6f
                          ? glm::normalize(up)
                          : glm::vec3(0.0f, 1.0f, 0.0f);
  body->applyCentralImpulse(asBtVector3(up_axis * Config::player_jump_strength * body->getMass()));
}