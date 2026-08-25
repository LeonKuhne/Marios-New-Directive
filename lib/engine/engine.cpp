#include "engine.h"
#include "lib/procgen/hallways.h"

void Engine::run()
{
  SDL_Log("Running");

  // floor
  ShapeData floor = scene.data_points.finishPlane(Config::floor);
  scene.shapes.add(new Shape(floor));

  // hallways
  generateHallways(scene, floor);

  while (running)
  {
    scene.tick();
    scene.render();
    controls.handle();
  }
}