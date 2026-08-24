#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <optional>
#include <SDL3/SDL.h>
#include <btBulletDynamicsCommon.h>
#include "mesh.h"

struct ShapeData
{
  ushort type;
  glm::vec3 pos = glm::vec3(0.0f);
  glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f);
  float density = 0.0f;
  glm::vec3 linear_velocity = glm::vec3(0.0f);
  glm::vec3 angular_velocity = glm::vec3(0.0f);
  SDL_FColor color;
  Mesh mesh;
  std::optional<btTransform> parent_transform;
  std::optional<glm::vec3> parent_center_offset;
};