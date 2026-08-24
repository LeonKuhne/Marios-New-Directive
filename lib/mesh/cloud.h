#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "lib/util/vertex.h"

static void cloudAddPoints(std::vector<glm::vec3> &all_vertices, ushort count, float min_radius)
{
  for (ushort i = 0; i < count; i++)
  {
    glm::vec3 vertex = glm::normalize(random_vertex());
    float radius = min_radius + ((float)rand() / (float)RAND_MAX) * (1.0f - min_radius);
    all_vertices.push_back(vertex * radius);
  }
}
