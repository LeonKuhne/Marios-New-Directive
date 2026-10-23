#pragma once

#include "lib/pbr/pbr_pipeline.h"
#include <SDL3/SDL_gpu.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include "lib/shapes/gameobjects/shape.h"
#include "lib/shapes/gameobjects/solid.h"

class Context;
class Scene;
class Shape;
struct ShapeData;

class ShapeManager
{
  Context &ctx;

public:
  std::vector<Shape*> shapes;
  std::vector<Solid*> visible_shapes;

  ShapeManager(Context &ctx) : ctx(ctx) {};
  ~ShapeManager();

  void add(Shape *shape);
  void add(Solid *solid);
  Shape* add(ShapeData& data);
  void remove(Shape *shape);
  void collect(ShapeManager* target);

  template<typename T> void render(RenderEvent<T>& event)
  {
    for (auto shape : visible_shapes)
      event.render(shape->render_object);
  }
};
