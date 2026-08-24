#pragma once

#include "shape.h"
#include "shape_pipeline.h"
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <algorithm>

class ShapeManager
{
  Context &ctx;
  ShapePipeline shape_pipeline;

public:
  std::vector<Shape *> shapes;
  Shape* selected = nullptr;

  ShapeManager(Context &ctx) : ctx(ctx), shape_pipeline(ShapePipeline(ctx)) {};
  ~ShapeManager();

  void add(Shape *shape);
  void remove(Shape *shape);
  void select(Shape *shape);
  void render(Frame &frame, SDL_GPURenderPass *pass);

#ifdef DRAW_GIZMOS
  Shape *gizmo;
  std::vector<std::function<btVector3()>> gizmo_positions;
  void addVertexGizmos(Shape *shape);
  void addGizmo(std::function<btVector3()> getPosition)
  {
    gizmo_positions.push_back(getPosition);
  }
#endif
};
