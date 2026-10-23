#pragma once

#include "lib/shapes/shape_data.h"
#include "lib/shapes/shape_manager.h"
#include "portal.h"
#include <SDL3/SDL_gpu.h>
#include <set>
#include "lib/pipeline/render_event.h"

class Room {
public:
  std::set<Room*> visible_rooms;
  std::vector<Portal*> portals;
  ShapeManager shapes;

  Room(Context& ctx) : shapes(ShapeManager(ctx)) { portals.reserve(4); };

  void addSurface(ShapeData& shape_data);
  void addPortal(Portal* portal);
  void updateVisibility();
  bool isVisibleFrom(const Room& observer) const;
  template<typename T> void render(RenderEvent<T>& event) { shapes.render(event); };
};