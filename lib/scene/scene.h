#pragma once

#include <btBulletDynamicsCommon.h>
#include <vector>
#include "camera.h"
#include "lib/render/context.h"
#include "lib/shapes/shape.h"
#include "lib/mesh/poly.h"
#include "lib/shapes/shape_data.h"
#include "lib/shapes/shape_manager.h"
#include "lib/gpu/transfer_vec.h"
#include "lib/render/frame.h"
#include "lib/render/window.h"
#include "lib/line/line_pipeline.h"
#include "lib/controls/mouse.h"
#include "player.h"

class Knife;

class Scene
{
public:
  Context ctx;
  Window window;
  DataPoints data_points;
  Player player;
  Camera camera;
  Frame frame;
  ShapeManager shapes;
  LinePipeline line_pipeline;
  bool &running;
  float sim_speed = 1.0f;
  ulong ticks = 0;

  Scene(bool &running, Mouse &mouse);

  void setup(Knife &knife, Mouse &mouse);
  void tick();
  void render() { frame.run(); }
  Shape *spawnAsteroid();

private:
  void gravityTick(btScalar timeStep);
  void checkCollision(btPersistentManifold *const &manifold);
};