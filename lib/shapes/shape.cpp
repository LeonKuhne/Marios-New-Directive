#include "shape.h"

Shape::Shape(const ShapeData info)
    : color(info.color),
      mesh(info.mesh),
      scale(info.scale),
      density(info.density),
      type(info.type)
{
  std::unordered_set<ushort> unique_indices = vec2set<ushort>(info.mesh.indices);
  bool has_collider = density != 0.0f;
  is_static = density > 100.0f;

  // compute centered vertices
  centered_vertices = subSet(unique_indices, info.mesh.all_vertices);
  glm::vec3 mesh_center = vecAvg(centered_vertices);
  for (glm::vec3 &vertex : centered_vertices)
    vertex -= mesh_center;
  center_offset = mesh_center;

  // physics attributes
  btCollisionShape *collider = nullptr;
  float mass = 0.0f;
  btVector3 localInertia(0, 0, 0);
  Volume mesh_volume({mesh_center, info.mesh, centered_vertices});
  volume = mesh_volume.sum;

  // create collider
  if (has_collider)
  {
    collider = new btConvexHullShape((btScalar *)centered_vertices.data(), centered_vertices.size(), sizeof(glm::vec3));
    collider->setLocalScaling(asBtVector3(scale));

    // calculate mass
    if (!is_static)
    {
      mass = volume * density;
      collider->calculateLocalInertia(mass, localInertia);
    }
  }

  // set transform
  btTransform start_transform;
  start_transform.setIdentity();
  start_transform.setRotation(asBtQuaternion(info.rotation));
  btVector3 pos = asBtVector3(info.pos);
  if (info.parent_transform.has_value())
  {
    glm::vec3 parent_center = info.parent_center_offset.value_or(glm::vec3(0.0f));
    glm::vec3 center_delta = (mesh_center - parent_center) * scale;
    pos = (*info.parent_transform) * asBtVector3(center_delta);
  }
  start_transform.setOrigin(pos);

  // create rigid body from collider
  btDefaultMotionState *motionState = new btDefaultMotionState(start_transform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collider, localInertia);
  body = new btRigidBody(rbInfo);
  body->setLinearVelocity(asBtVector3(info.linear_velocity));
  body->setAngularVelocity(asBtVector3(info.angular_velocity));
  body->setUserPointer(this);
  body->setRestitution(0.0f);
  body->setFriction(Config::ground_friction);

  // disable collisions
  if (!has_collider)
  {
    body->setActivationState(DISABLE_DEACTIVATION);
    body->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
  }
}

Shape::~Shape()
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

void Shape::getTransform(btScalar *transform_matrix)
{
    btTransform transform;
    body->getMotionState()->getWorldTransform(transform);
    transform.getOpenGLMatrix(transform_matrix);

    // Apply shape scale to the model matrix
    for (int i = 0; i < 3; i++)
    {
        transform_matrix[i * 4 + 0] *= scale[i];
        transform_matrix[i * 4 + 1] *= scale[i];
        transform_matrix[i * 4 + 2] *= scale[i];
    }
}

void Shape::setPosition(btVector3 pos)
{
  btTransform transform;
  body->getMotionState()->getWorldTransform(transform);
  transform.setOrigin(pos);
  body->getMotionState()->setWorldTransform(transform);
}

glm::vec3 Shape::inLocalSpace(const glm::vec3 global_pos)
{
  btVector3 local_intersect = body->getWorldTransform().inverse() * asBtVector3(global_pos);
  glm::vec3 intersect = asGlmVec3(local_intersect);
  intersect /= scale;
  intersect += center_offset;
  return intersect;
}

glm::vec3 Shape::inGlobalSpace(const glm::vec3 local_pos)
{
  glm::vec3 pos = local_pos - center_offset;
  pos *= scale;
  btVector3 global_intersect = body->getWorldTransform() * asBtVector3(pos);
  return asGlmVec3(global_intersect);
}

ShapeData Shape::getShapeDataStarter()
{
  btTransform transform;
  body->getMotionState()->getWorldTransform(transform);
  return ShapeData{
      .pos = asGlmVec3(transform.getOrigin()),
      .rotation = asGlmQuat(transform.getRotation()),
      .scale = scale,
      .density = density,
      .linear_velocity = asGlmVec3(body->getLinearVelocity()),
      .angular_velocity = asGlmVec3(body->getAngularVelocity()),
      .color = color,
  };
}