#pragma once

#include "shape.h"
#include "lib/pipeline/pbr_pipeline.h"
#include <btBulletDynamicsCommon.h>
#include <vector>

class ShapeManager
{
  Context &ctx;
  PBRPipeline pbr_pipeline;

public:
  std::vector<Shape *> shapes;
  Shape* selected = nullptr;

  ShapeManager(Context &ctx) : ctx(ctx), pbr_pipeline(PBRPipeline(ctx)) {};
  ~ShapeManager();

  void add(Shape *shape);
  void add(ShapeData& data);
  void remove(Shape *shape);
  void select(Shape *shape);
  void render(Frame &frame, SDL_GPURenderPass *pass);
};
