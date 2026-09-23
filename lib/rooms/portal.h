#pragma once

#include "lib/shapes/shape.h"
#include "portal_path.h"

class Room;

class Portal {
public:
  glm::vec3 center;
  std::vector<glm::vec3> vertices;
  Room& destination;

  Portal(Shape* shape, Room& destination);

  void eachTargetPlane(Portal& destination, const std::vector<glm::vec3>& source_vertices,
                       const std::vector<glm::vec3>& destination_vertices,
                       const std::function<void(const glm::mat3)>& callback);

private:
  glm::vec3 furthestVertexInDirection(glm::vec3 target_direction, const std::vector<glm::vec3>& vertices);
};
