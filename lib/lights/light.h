#pragma once
#include <glm/glm.hpp>

struct Light
{
  glm::vec3 pos;
  float _pad0;

  glm::vec3 color = {1.0f, 1.0f, 1.0f};
  float _pad1;

  float intensity = 1000.0f;
  float _pad2[3];
};