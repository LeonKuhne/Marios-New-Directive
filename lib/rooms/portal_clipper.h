#pragma once

#include "portal_path.h"
#include <functional>
#include <vector>

class PortalClipper
{
public:
  static void eachPlane(const PortalPath& path,
                        const std::function<void(const glm::mat3)>& callback,
                        bool include_reverse_planes = true);
  static std::vector<glm::vec3> clip(const glm::mat3& clipping_plane,
                                     const std::vector<glm::vec3>& target_polygon);
};
