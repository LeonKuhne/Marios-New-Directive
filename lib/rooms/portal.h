#pragma once

#include "lib/shapes/gameobjects/trigger.h"
#include "portal_path.h"

class Room;

class Portal {
public:
  glm::vec3 center;
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> reverse_vertices;
  Trigger* trigger;
  Room& room_a;
  Room& room_b;

  Portal(Trigger* trigger, Room& room_a, Room& room_b);

  glm::vec3 normal() const;
  Room& otherRoom(const Room& room) const;
  const std::vector<glm::vec3>& verticesFor(const Room& room) const;

  void eachTargetPlane(Portal& destination, const std::vector<glm::vec3>& source_vertices,
                       const std::vector<glm::vec3>& destination_vertices,
                       const std::function<void(const glm::mat3)>& callback);

private:
  glm::vec3 furthestVertexOppositeDirection(glm::vec3 target_direction, const std::vector<glm::vec3>& vertices);
};
