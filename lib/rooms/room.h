#pragma once

#include "lib/shapes/shape_data.h"
#include "lib/shapes/shape_manager.h"
#include "portal.h"
#include <set>

class Room {
private:
  std::set<Room*> visible_rooms;

public:
  std::vector<Portal*> portals;
  ShapeManager shapes;

  Room(Context& ctx) : shapes(ShapeManager(ctx)) { portals.reserve(4); };

  void addSurface(ShapeData& shape_data);
  void addPortal(Portal& portal);
  void updateVisibility();
  void render(Scene& scene, SDL_GPURenderPass *pass);
  bool isVisibleFrom(const Room& observer) const;
};