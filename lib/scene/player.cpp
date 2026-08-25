#include "player.h"
#include <glm/ext/vector_float3.hpp>

Player::Player(PlayerInfo info)
{
  btVector3 localInertia;
  btScalar capsule_height = info.height - 2.0f * info.radius;
  btCollisionShape *capsule_collider = new btCapsuleShape(info.radius, capsule_height);
  capsule_collider->calculateLocalInertia(info.mass, localInertia);

  auto *player_collider = new btCompoundShape();
  btTransform offset;
  offset.setIdentity();
  offset.setOrigin(btVector3(0.0f, info.height * 0.5f, 0.0f));
  player_collider->addChildShape(offset, capsule_collider);

  btTransform transform;
  transform.setIdentity();
  transform.setRotation(asBtQuaternion(info.rotation));
  transform.setOrigin(asBtVector3(info.pos));

  btDefaultMotionState *motionState = new btDefaultMotionState(transform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(info.mass, motionState, player_collider, localInertia);
  body = new btRigidBody(rbInfo);
  body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
  body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
  body->setDamping(info.linear_damping, info.angular_damping);
  body->setFriction(Config::player_friction);
  body->setRollingFriction(0.0f);
  body->setRestitution(0.0f);
  body->setUserPointer(this);
  body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}

void Player::tick() {}

void Player::move(const glm::vec2 &direction)
{
  btVector3 velocity = body->getLinearVelocity();
  glm::vec2 velocity_xy = glm::vec2(velocity.getX(), velocity.getZ());

  glm::vec3 forward_dir = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
  glm::vec3 right_dir = glm::vec3(forward_dir.z, 0.0f, -forward_dir.x);

  // add delta
  glm::vec3 delta = forward_dir * direction.x + right_dir * direction.y;
  float move_accel = isSprinting ? Config::player_sprint_accel : Config::player_walk_accel;
  if (!isGrounded)
    move_accel *= 0.5f; // reduce move speed when in the air

  // limit move speed
  if (glm::length(velocity_xy) < Config::player_max_speed)
    velocity_xy += glm::normalize(glm::vec2(delta.x, delta.z)) * move_accel;

  // update velocity
  velocity.setX(velocity_xy.x);
  velocity.setZ(velocity_xy.y);
  body->activate(true);
  body->setLinearVelocity(velocity);
}

void Player::jump()
{
  if (!isGrounded)
    return;

  glm::vec3 up_axis = glm::dot(up, up) > 1e-6f
                          ? glm::normalize(up)
                          : glm::vec3(0.0f, 1.0f, 0.0f);
  body->activate(true);
  body->applyCentralImpulse(asBtVector3(up_axis * Config::player_jump_strength * body->getMass()));
}

void Player::setupGroundedListener()
{
    gContactAddedCallback = [](btManifoldPoint &cp, 
      const btCollisionObjectWrapper *a, int partIdA, int indexA,
      const btCollisionObjectWrapper *b, int partIdB, int indexB)
    {

        Player *player = Player::isPlayerCollidingWithGround(a->getCollisionObject(), b->getCollisionObject());
        if (!player)
          return true;

        btVector3 normal = cp.m_normalWorldOnB;

        // Make normal point from the ground toward the player.
        if (b->getCollisionObject()->getUserPointer() == player)
          normal = -normal;

        if (normal.getY() > 0.5f)
          player->isGrounded = true;
        return true;
    };

    gContactEndedCallback = [](btPersistentManifold *const &manifold)
    {
        Player *player = Player::isPlayerCollidingWithGround(manifold->getBody0(), manifold->getBody1());
        if (player)
            player->isGrounded = false;
    };
}

Player *Player::isPlayerCollidingWithGround(const btCollisionObject *a, const btCollisionObject *b)
{
    if (!a->getUserPointer() || !b->getUserPointer())
        return nullptr;

    auto *shape_a = static_cast<ShapeBase *>(a->getUserPointer());
    auto *shape_b = static_cast<ShapeBase *>(b->getUserPointer());

    if (shape_a->getType() == ShapeType::PLANE &&
        shape_b->getType() == ShapeType::PLAYER)
        return static_cast<Player *>(b->getUserPointer());

    if (shape_a->getType() == ShapeType::PLAYER &&
        shape_b->getType() == ShapeType::PLANE)
        return static_cast<Player *>(a->getUserPointer());

    return nullptr;
}

Player *Player::isPlayerGravitonCollision(const btCollisionObject *a, const btCollisionObject *b)
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
