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
  void add(ShapeData& data);
  void remove(Shape *shape);
  void select(Shape *shape);
  void render(Frame &frame, SDL_GPURenderPass *pass);
};
