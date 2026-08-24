#pragma once

#include <vector>
#include <unordered_set>

template <typename T>
static std::unordered_set<T> vec2set(const std::vector<T> &indices)
{
  std::unordered_set<T> unique_indices;
  unique_indices.reserve(indices.size());
  for (T index : indices)
    unique_indices.insert(index);
  return unique_indices;
}

template <typename T>
static std::vector<T> subSet(const std::unordered_set<ushort> &indices, const std::vector<T> *values)
{
  std::vector<T> subset;
  for (ushort index : indices)
    subset.push_back(values->at(index));
  return subset;
}

static glm::vec3 vecAvg(const std::vector<glm::vec3> &unique_vertices)
{
  glm::vec3 center = glm::vec3(0.0f);
  for (glm::vec3 vertex : unique_vertices)
    center += vertex;
  center /= static_cast<float>(unique_vertices.size());
  return center;
}