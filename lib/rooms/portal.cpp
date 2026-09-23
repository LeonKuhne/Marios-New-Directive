#include "portal.h"
#include "room.h"
#include <algorithm>
#include <limits>
#include <unordered_set>

Portal::Portal(Shape* shape, Room& room_a, Room& room_b)
  : shape(shape), room_a(room_a), room_b(room_b) {
  // assign vertices and compute center
  center = glm::vec3(0.0f);
  // todo use global coordinates here instead of local
  std::unordered_set<ushort> seen_indices;
  for (size_t vertex_index : shape->mesh.indices)
  {
    if (!seen_indices.emplace(vertex_index).second)
      continue;
    glm::vec3 local_vertex = shape->mesh.all_vertices->at(vertex_index);
    glm::vec3 global_vertex = shape->inGlobalSpace(local_vertex);
    vertices.emplace_back(global_vertex);
    center += global_vertex;
  }
  center /= vertices.size();
  reverse_vertices = vertices;
  std::reverse(reverse_vertices.begin(), reverse_vertices.end());
}

glm::vec3 Portal::normal() const
{
  return glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
}

Room& Portal::otherRoom(const Room& room) const
{
  return &room == &room_a ? room_b : room_a;
}

const std::vector<glm::vec3>& Portal::verticesFor(const Room& room) const
{
  return &room == &room_a ? vertices : reverse_vertices;
}

void Portal::eachTargetPlane(Portal& destination, const std::vector<glm::vec3>& source_vertices,
                             const std::vector<glm::vec3>& destination_vertices,
                             const std::function<void(const glm::mat3)>& callback)
{
  glm::vec3 source_center(0.0f);
  for (const glm::vec3& vertex : source_vertices)
    source_center += vertex;
  source_center /= source_vertices.size();

  for (int i = 0; i < source_vertices.size(); i++)
  {
    glm::vec3 a = source_vertices[i];
    glm::vec3 b = source_vertices[(i + 1) % source_vertices.size()];
    glm::vec3 average_vertex = (a + b) / 2.0f;
    glm::vec3 average_displacement = average_vertex - source_center;
    glm::vec3 destination_vertex = destination.furthestVertexInDirection(average_displacement, destination_vertices);
    if (destination_vertex == a || destination_vertex == b)
        continue;
    callback(glm::mat3(a, b, destination_vertex));
  }
}

glm::vec3 Portal::furthestVertexInDirection(glm::vec3 target_direction, const std::vector<glm::vec3>& vertices)
{
  int closest_idx;
  float max_dot = -std::numeric_limits<float>::infinity(); // closest to 1 means most aligned
  for (int i=0; i<vertices.size(); i++)
  {
    glm::vec3 displacement = vertices[i] - center;
    float dot = glm::dot(target_direction, displacement);
    if (dot < max_dot)
      continue;
    closest_idx = i;
    max_dot = dot;
  }
  return vertices[closest_idx];
}