#pragma once

#include <SDL3/SDL_gpu.h>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <sys/types.h>
#include <unordered_set>
#include <set>
#include <optional>
#include "lib/render/context.h"
#include "lib/util/vertex.h"
#include "shape_data.h"
#include "shape_base.h"
#include "util.h"
#include "volume.h"
#include "mesh.h"

class Shape : public ShapeBase
{
private:
  std::vector<glm::vec3> centered_vertices;
  ushort type;

public:
  btRigidBody *body;
  glm::vec3 center_offset = glm::vec3(0.0f);
  glm::vec3 scale;
  SDL_FColor color;
  Mesh mesh;
  bool selected = false;
  float volume = 0.0f;
  float density;
  bool is_static;

  Shape(const ShapeData info);
  ~Shape();

  void assignToWorld(btDiscreteDynamicsWorld *world) { world->addRigidBody(body); }
  void getTransform(glm::mat4 &transform_matrix);
  void setPosition(btVector3 pos);
  glm::vec3 inLocalSpace(const glm::vec3 global_pos);
  glm::vec3 inGlobalSpace(const glm::vec3 local_pos);
  ShapeData getShapeDataStarter();
  float getMass() { return body->getMass(); }
  ushort getType() override { return type; }
};