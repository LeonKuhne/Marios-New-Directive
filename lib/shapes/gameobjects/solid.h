#pragma once

#include "lib/pbr/pbr_pipeline.h"
#include "lib/pbr/pbr_render_object.h"
#include "shape.h"
#include "lib/collisions/collider_body.h"

class Solid : public Shape
{
protected:
  btCollisionShape *collider = nullptr;

public:
  ColliderBody* body;
  PBRRenderObject render_object;

  struct Info
  {
    ShapeData shape;
    float density;
  };

  Solid(const Info &info);
  ~Solid();

  void assignToWorld(btDiscreteDynamicsWorld *world) { world->addRigidBody(body); }
  float getMass() { return body->getMass(); }
  void getTransform(glm::mat4 &transform_matrix);
  void setPosition(btVector3 pos);
  glm::vec3 inLocalSpace(const glm::vec3 global_pos);
  glm::vec3 inGlobalSpace(const glm::vec3 local_pos);

private:
  void updateRenderObject();
};