#pragma once

#include <vector>
#include <unordered_set>
#include <array>
#include <glm/glm.hpp>
#include <numeric>
#include <sys/types.h>
#include "mesh.h"

struct VolumeInfo
{
  const glm::vec3 center;
  const Mesh mesh;
  const std::vector<glm::vec3> &unique_vertices;
};

class Volume
{
  const glm::vec3 center;
  const Mesh mesh;
  const std::vector<glm::vec3> &unique_vertices;
  std::vector<std::array<glm::vec3, 3>> triangles;
  std::vector<float> tetra_volumes;

public:
  float sum;
  glm::vec3 centroid;

  Volume(VolumeInfo info) : center(info.center), mesh(info.mesh), unique_vertices(info.unique_vertices)
  {
    computeCenteredTriangles();
    computeTetraVolumes();
    sum = std::reduce(tetra_volumes.begin(), tetra_volumes.end(), 0.0f);
    computeCentroid();
  }

private:
  void computeCenteredTriangles()
  {
    for (size_t i = 0; i + 2 < mesh.indices.size(); i += 3)
    {
      const glm::vec3 a = mesh.all_vertices->at(mesh.indices[i]) - center;
      const glm::vec3 b = mesh.all_vertices->at(mesh.indices[i + 1]) - center;
      const glm::vec3 c = mesh.all_vertices->at(mesh.indices[i + 2]) - center;
      triangles.push_back({a, b, c});
    }
  }

  void computeTetraVolumes()
  {
    for (auto tri : triangles)
      tetra_volumes.push_back(glm::abs(glm::dot(tri[0], glm::cross(tri[1], tri[2]))) / 6.0f);
  }

  void computeCentroid()
  {
    glm::vec3 centroid_sum(0.0f);
    for (size_t i = 0; i < triangles.size(); ++i)
    {
      auto tri = triangles[i];
      const glm::vec3 tetra_centroid = center + (tri[0] + tri[1] + tri[2]) * 0.25f;
      centroid_sum += tetra_centroid * tetra_volumes[i];
    }
    centroid = centroid_sum / sum;
  }
};
