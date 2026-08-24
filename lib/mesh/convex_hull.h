#pragma once

#include <sys/types.h>
#include <vector>
#include <unordered_set>
#include <glm/glm.hpp>

struct ConvexHullInfo {
  std::vector<ushort> *indices;
  const std::unordered_set<ushort> &point_cloud;
  const std::vector<glm::vec3> *all_vertices;
};

void convexHull(ConvexHullInfo info);