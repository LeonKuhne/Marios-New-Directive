#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/mat3x3.hpp>
#include <vector>

class Portal;
class Room;

class PortalPath
{
public:
  PortalPath(Portal& first_portal, Portal& last_portal,
             const std::vector<glm::vec3>& source_vertices,
             const std::vector<glm::vec3>& clipped_vertices);

  PortalPath transitionTo(Portal& next_portal,
                          const std::vector<glm::vec3>& next_vertices) const;
  PortalPath childTo(Portal& next_portal,
                     const std::vector<glm::vec3>& next_vertices) const;

  void addVisitedRoom(Room& room);
  bool hasVisitedRoom(const Room& room) const;
  void addClippingPlane(const glm::mat3& plane);
  void appendClippingPlanes(std::vector<glm::mat3>&& planes);

  Portal& first_portal;
  Portal& last_portal;
  std::vector<glm::vec3> source_vertices;
  std::vector<glm::vec3> clipped_vertices;
  std::vector<glm::mat3> clipping_planes;
  std::vector<Room*> visited_rooms;
};