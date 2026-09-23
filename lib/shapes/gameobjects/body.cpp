#include "body.h"
#include "lib/shapes/volume.h"
#include "lib/util/vertex.h"
#include "lib/engine/config.h"

Body::Body(const Info &info)
  : Solid(info.solid)
{
  // physics attributes
  Volume mesh_volume({.center=center_offset, .mesh=info.solid.shape.mesh, .unique_vertices=centered_vertices});
  float volume = mesh_volume.sum;

  float mass = volume * density;
  btVector3 localInertia(0, 0, 0);

  collider->calculateLocalInertia(mass, localInertia);

  // create rigid body from collider
  btDefaultMotionState *motionState = new btDefaultMotionState(transform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collider, localInertia);
  body = new ColliderBody(info.solid.shape.collider_type, rbInfo);
  body->setLinearVelocity(asBtVector3(info.solid.shape.linear_velocity));
  body->setAngularVelocity(asBtVector3(info.solid.shape.angular_velocity));
  body->setRestitution(0.0f);
  body->setFriction(Config::ground_friction);
}