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

public:
  std::vector<Shape*> shapes;
  std::vector<Shape*> visible_shapes;

  ShapeManager(Context &ctx) : ctx(ctx) {};
  ~ShapeManager();

  void add(Shape *shape);
  Shape* add(ShapeData& data);
  void remove(Shape *shape);
  void render(Scene& scene, SDL_GPURenderPass *pass);
};
