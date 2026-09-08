#pragma once

#include <btBulletDynamicsCommon.h>
#include "camera.h"
#include "lib/lights/light_manager.h"
#include "lib/mesh/plane_builder.h"
#include "lib/pbr/pbr_materials.h"
#include "lib/pbr/pbr_vertices.h"
#include "lib/render/context.h"
#include "lib/shapes/shape_manager.h"
#include "lib/render/frame.h"
#include "lib/render/window.h"
#include "lib/controls/mouse.h"
#include "player.h"

class Scene
{
public:
  Context ctx;
  Window window;
  std::vector<glm::vec3> all_vertices;

  // game objects
  Player player;
  Camera camera;

  // gpu storage
  PlaneBuilder plane_builder;
  PBRVertices pbr_vertices;
  PBRMaterials pbr_materials;
  
  // rendering
  Frame frame;
  ShapeManager shapes;
  LightManager light_manager;

  // game state
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