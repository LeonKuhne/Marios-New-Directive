#include "solid.h"
#include "lib/util/vertex.h"
#include <glm/gtc/type_ptr.hpp>

Solid::Solid(const Solid::Info &info)
  : Shape(info.shape)
{
  // create body
  btRigidBody::btRigidBodyConstructionInfo rbInfo(
    0.0f, 
    new btDefaultMotionState(transform), 
    collider, 
    btVector3(0,0,0));
  body = new ColliderBody(info.shape.collider_type, rbInfo);

  // create collider
  collider = new btConvexHullShape((btScalar *)centered_vertices.data(), (int) centered_vertices.size(), sizeof(glm::vec3));
  collider->setLocalScaling(asBtVector3(scale));
}

Solid::~Solid()
{
  if (!body)
    return;

  btMotionState *motion_state = body->getMotionState();
  btCollisionShape *collision_shape = body->getCollisionShape();

  delete body;
  delete motion_state;
  delete collision_shape;
  body = nullptr;
}

void Solid::getTransform(glm::mat4 &transform_matrix)
{
    btTransform transform;
    body->getMotionState()->getWorldTransform(transform);
    transform.getOpenGLMatrix(glm::value_ptr(transform_matrix));

    // Apply shape scale to the model matrix
    transform_matrix[0] *= scale.x;
    transform_matrix[1] *= scale.y;
    transform_matrix[2] *= scale.z;
}

void Solid::setPosition(btVector3 pos)
{
  btTransform transform;
  body->getMotionState()->getWorldTransform(transform);
  transform.setOrigin(pos);
  body->getMotionState()->setWorldTransform(transform);
}

glm::vec3 Solid::inLocalSpace(const glm::vec3 global_pos)
{
  btVector3 local_intersect = body->getWorldTransform().inverse() * asBtVector3(global_pos);
  glm::vec3 intersect = asGlmVec3(local_intersect);
  intersect /= scale;
  intersect += center_offset;
  return intersect;
}

glm::vec3 Solid::inGlobalSpace(const glm::vec3 local_pos)
{
  glm::vec3 pos = local_pos - center_offset;
  pos *= scale;
  btVector3 global_intersect = body->getWorldTransform() * asBtVector3(pos);
  return asGlmVec3(global_intersect);
}