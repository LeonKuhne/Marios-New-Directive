#include "engine.h"
#include "lib/mesh/cube.h"

void Engine::run()
{
  SDL_Log("Running");

  //scene.shapes.add(new Shape(scene.data_points.finishCube(Config::floor)));

  // floor
  scene.shapes.add(new Shape(scene.data_points.finishPlane(Config::floor)));

  while (running)
  {
    scene.tick();
    scene.render();
    controls.handle();
  }
}