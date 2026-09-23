#include "room.h"
#include "portal_clipper.h"
#include <utility>
#include "portal_path.h"

void Room::addSurface(ShapeData& shape_data) {
  shapes.add(shape_data);
}

void Room::addPortal(ShapeData& shape_data, Room& destination) {
  Shape* shape = shapes.add(shape_data);
  portals.emplace_back(shape, destination);

  // todo
  // add a callback to an on collision start between player and portal that sets both rooms connected to the portal as active rooms
  // add a callback to an on collision end between player and portal that removes the room that is on the far side of the portal plane from active rooms
}

void Room::updateVisibility()
{
  visible_rooms.clear();
  std::vector<PortalPath> paths;
  for (Portal& starting_portal : portals)
    for (Portal& target_portal : starting_portal.destination.portals)
    {
      PortalPath& path = paths.emplace_back(starting_portal, target_portal,
        starting_portal.vertices, target_portal.vertices);
      path.addVisitedRoom(*this);
      path.addVisitedRoom(starting_portal.destination);
      path.addVisitedRoom(target_portal.destination);
      PortalClipper::eachPlane(path, [&](const glm::mat3& plane) {
        path.addClippingPlane(plane);
      }, false);
      if (&starting_portal.destination != this)
        visible_rooms.emplace(&starting_portal.destination);
      if (&target_portal.destination != this)
        visible_rooms.emplace(&target_portal.destination);
    }

  // recurse clipping planes
  while (paths.size() > 0)
  {
    PortalPath path = std::move(paths.back());
    paths.pop_back();

    std::vector<std::pair<Portal&, std::vector<glm::vec3>>> portals_to_check;
    for (Portal& target : path.last_portal.destination.portals)
    {
      // ignore visited rooms
      if (path.hasVisitedRoom(target.destination))
        continue;
      portals_to_check.emplace_back(target, target.vertices);
    }

    // clip the destination portals against the complete portal frustum
    for (const glm::mat3& plane : path.clipping_planes)
    {
      for (std::pair<Portal&, std::vector<glm::vec3>>& clipped_target : portals_to_check)
      {
        if (clipped_target.second.empty())
          continue;

        // clip the next portals vertices to see if visible
        clipped_target.second = PortalClipper::clip(plane, clipped_target.second);
      }
    }

    // collect visible end portals to recurse
    for (auto [next_portal, clipped_vertices] : portals_to_check)
    {
      if (clipped_vertices.empty())
        continue;
      PortalPath transition = path.transitionTo(next_portal, clipped_vertices);
      std::vector<glm::mat3> transition_planes;
      PortalClipper::eachPlane(transition, [&](const glm::mat3& plane) {
        transition_planes.emplace_back(plane);
      });
      // The clipped portal pair can have different vertex counts. Keep the
      // forward-facing half plus its boundary plane from the reverse set.
      transition_planes.resize(transition_planes.size() / 2);
      PortalPath next_path = path.childTo(next_portal, clipped_vertices);
      Room& visible_room = next_portal.destination;
      next_path.appendClippingPlanes(std::move(transition_planes));
      next_path.addVisitedRoom(visible_room);
      paths.emplace_back(std::move(next_path));
      visible_rooms.emplace(&visible_room);
    }
  }
}

void Room::render(Scene& scene, SDL_GPURenderPass *render_pass)
{
  shapes.render(scene, render_pass);
  for (Room* visible_room : visible_rooms)
    visible_room->shapes.render(scene, render_pass);
}

bool Room::isVisibleFrom(const Room& observer) const
{
  return this == &observer || observer.visible_rooms.contains(const_cast<Room*>(this));
}
