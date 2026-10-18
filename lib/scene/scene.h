#pragma once

#include <SDL3/SDL_timer.h>
#include <btBulletDynamicsCommon.h>
#include "camera.h"
#include "lib/collisions/collision_manager.h"
#include "lib/lights/light_manager.h"
#include "lib/mesh/plane_builder.h"
#include "lib/pbr/pbr_materials.h"
#include "lib/pbr/pbr_vertices.h"
#include "lib/pbr/pbr_pipeline.h"
#include "lib/render/context.h"
#include "lib/gui/gui.h"
#include "lib/rooms/room_manager.h"
#include "lib/shapes/shape_manager.h"
#include "lib/render/frame.h"
#include "lib/render/window.h"
#include "lib/controls/mouse.h"

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
  LightManager light_manager;
  CollisionManager collision_manager;

  // render pipelines
  PBRPipeline pbr_pipeline;

  // render composites
  GUI gui;
  RoomManager room_manager;

  // game state
  bool &running;
  double performance_frequency = static_cast<double>(SDL_GetPerformanceFrequency());
  Uint64 last_tick_time = SDL_GetPerformanceCounter();
  Uint64 last_render_time = SDL_GetPerformanceCounter();

  Scene(bool &running, Mouse &mouse);

  void setup(Mouse &mouse);
  void tick();
  void render();

private:
  void gravityTick(btScalar timeStep);
  void updateGPUData();
};