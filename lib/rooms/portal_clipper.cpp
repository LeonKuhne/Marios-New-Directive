#include "portal_clipper.h"
#include "portal.h"
#include <algorithm>
#include <utility>

void PortalClipper::eachPlane(const PortalPath& path,
                              const std::function<void(const glm::mat3)>& callback,
                              bool include_reverse_planes)
{
  auto orient_plane = [&](const glm::mat3& plane, const glm::vec3& keep_point) {
    glm::mat3 oriented_plane = plane;
    glm::vec3 normal = glm::cross(plane[1] - plane[0], plane[2] - plane[0]);
    if (glm::dot(normal, keep_point - plane[0]) < 0.0f)
      std::swap(oriented_plane[0], oriented_plane[1]);
    callback(oriented_plane);
  };

  // Use two vertices from the source portal and one from the destination.
  glm::vec3 destination_center(0.0f);
  for (const glm::vec3& vertex : path.clipped_vertices)
    destination_center += vertex;
  destination_center /= path.clipped_vertices.size();
  path.first_portal.eachTargetPlane(path.last_portal, path.source_vertices, path.clipped_vertices,
    [&](const glm::mat3& plane) { orient_plane(plane, destination_center); });

  if (include_reverse_planes)
  {
    // Use two vertices from the destination portal and one from the source.
    path.last_portal.eachTargetPlane(path.first_portal, path.clipped_vertices, path.source_vertices,
      [&](const glm::mat3& plane) { orient_plane(plane, path.first_portal.center); });
  }
}

std::vector<glm::vec3> PortalClipper::clip(const glm::mat3& clipping_plane,
                                           const std::vector<glm::vec3>& target_polygon)
{
  const glm::vec3& a = clipping_plane[0];
  const glm::vec3& b = clipping_plane[1];
  const glm::vec3& c = clipping_plane[2];
  glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));

  auto distance = [&](const glm::vec3& point) {
    return glm::dot(normal, point - a);
  };

  constexpr float clip_epsilon = 1e-4f;
  std::vector<glm::vec3> result;
  result.reserve(target_polygon.size() + 2);

  for (size_t i = 0; i < target_polygon.size(); ++i)
  {
    const glm::vec3& current = target_polygon[i];
    const glm::vec3& next = target_polygon[(i + 1) % target_polygon.size()];
    float current_distance = distance(current);
    float next_distance = distance(next);
    bool current_inside = current_distance >= -clip_epsilon;
    bool next_inside = next_distance >= -clip_epsilon;

    if (current_inside && next_inside)
      result.push_back(next);
    else if (current_inside && !next_inside)
    {
      float t = current_distance / (current_distance - next_distance);
      result.push_back(current + t * (next - current));
    }
    else if (!current_inside && next_inside)
    {
      float t = current_distance / (current_distance - next_distance);
      result.push_back(current + t * (next - current));
      result.push_back(next);
    }
  }

  return result;
}
