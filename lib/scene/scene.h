#pragma once

#include <btBulletDynamicsCommon.h>
#include "camera.h"
#include "lib/lights/light_manager.h"
#include "lib/render/context.h"
#include "lib/shapes/shape_manager.h"
#include "lib/render/frame.h"
#include "lib/render/window.h"
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
  LightManager light_manager;
  bool &running;
  Uint64 last_game_time = SDL_GetPerformanceCounter();

  Scene(bool &running, Mouse &mouse);

  void setup(Mouse &mouse);
  void tick();
  void render() { frame.run(); }

private:
  void gravityTick(btScalar timeStep);
  void checkCollision(btPersistentManifold *const &manifold);
};