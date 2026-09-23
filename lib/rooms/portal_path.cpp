#include "portal_path.h"
#include "portal.h"
#include "room.h"
#include <algorithm>
#include <iterator>

PortalPath::PortalPath(Portal& first_portal, Portal& last_portal,
                       const std::vector<glm::vec3>& source_vertices,
                       const std::vector<glm::vec3>& clipped_vertices)
  : first_portal(first_portal),
    last_portal(last_portal),
    source_vertices(source_vertices),
    clipped_vertices(clipped_vertices)
{
}

PortalPath PortalPath::transitionTo(Portal& next_portal,
                                    const std::vector<glm::vec3>& next_vertices) const
{
  return PortalPath(last_portal, next_portal, clipped_vertices, next_vertices);
}

PortalPath PortalPath::childTo(Portal& next_portal,
                               const std::vector<glm::vec3>& next_vertices) const
{
  PortalPath child(first_portal, next_portal, clipped_vertices, next_vertices);
  child.clipping_planes = clipping_planes;
  child.visited_rooms = visited_rooms;
  return child;
}

void PortalPath::addVisitedRoom(Room& room)
{
  visited_rooms.emplace_back(&room);
}

bool PortalPath::hasVisitedRoom(const Room& room) const
{
  return std::ranges::contains(visited_rooms, &room);
}

void PortalPath::addClippingPlane(const glm::mat3& plane)
{
  clipping_planes.emplace_back(plane);
}

void PortalPath::appendClippingPlanes(std::vector<glm::mat3>&& planes)
{
  clipping_planes.insert(clipping_planes.end(),
                         std::make_move_iterator(planes.begin()),
                         std::make_move_iterator(planes.end()));
}
