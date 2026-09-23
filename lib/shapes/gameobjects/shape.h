#pragma once

#include <SDL3/SDL_gpu.h>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <sys/types.h>
#include "lib/shapes/mesh.h"
#include "lib/shapes/shape_data.h"
#include "lib/shapes/mesh.h"

class Shape
{
protected:
  std::vector<glm::vec3> centered_vertices;

public:
  glm::vec3 center_offset = glm::vec3(0.0f);

  SDL_FColor color;
  Mesh mesh;
  glm::vec3 scale;
  bool is_visible;
  btTransform transform;

  Shape(const ShapeData &info);
};