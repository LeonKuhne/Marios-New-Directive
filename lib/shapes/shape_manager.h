#pragma once

#include "lib/pbr/pbr_pipeline.h"
#include <btBulletDynamicsCommon.h>
#include <vector>

class Context;
class Scene;
class Shape;
struct ShapeData;

class ShapeManager
{
  Context &ctx;
  PBRPipeline pbr_pipeline;

public:
  std::vector<Shape *> shapes;

  ShapeManager(Context &ctx) : ctx(ctx), pbr_pipeline(PBRPipeline(ctx)) {};
  ~ShapeManager();

  void add(Shape *shape);
  void add(ShapeData& data);
  void remove(Shape *shape);
  void render(Scene& scene, SDL_GPURenderPass *pass);
};
