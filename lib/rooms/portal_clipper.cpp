#include "portal_clipper.h"
#include "portal.h"

void PortalClipper::eachPlane(const PortalPath& path, const std::function<void(const glm::mat3)>& callback)
{
  // Use two vertices from the source portal and one from the destination.
  path.first_portal.eachTargetPlane(path.last_portal, path.source_vertices, path.clipped_vertices, callback);
  // Use two vertices from the destination portal and one from the source.
  path.last_portal.eachTargetPlane(path.first_portal, path.clipped_vertices, path.source_vertices, callback);
}

std::vector<glm::vec3> PortalClipper::clip(const glm::vec3& first_portal_center, const glm::mat3& clipping_plane, const std::vector<glm::vec3>& target_polygon)
{
  // todo rewrite this in ways i understand 
  // 1. find points that are outside of the clipping plane using the first portals origin point to determine the side that should be clipped
  // 2. for outside points, remove these and replace them with intersection points inserting them in wrapping order where the removed points were before

  const glm::vec3& a = clipping_plane[0];
  const glm::vec3& b = clipping_plane[1];
  const glm::vec3& c = clipping_plane[2];
  glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));

  auto distance = [&](const glm::vec3& point) {
    return glm::dot(-normal, point - first_portal_center);
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

    if (current_inside != next_inside)
    {
        float t = current_distance / (current_distance - next_distance);
        result.push_back(current + t * (next - current));
    }

    if (next_inside)
        result.push_back(next);
  }

  return result;
}
