#pragma once
#include <glm/glm.hpp>

class Light
{
private:
  glm::vec4 pos;
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  float intensity = 1000.0f;
  float pad[3];
public:
  Light(const glm::vec3& pos, float intensity) : pos(pos, 1.0f), intensity(intensity) {}
};