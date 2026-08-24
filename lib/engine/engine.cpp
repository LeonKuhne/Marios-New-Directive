#include "engine.h"

void Engine::run()
{
  SDL_Log("Running");

  // add floor
  //scene.shapes.add(new Shape(scene.data_points.finishCube(Config::floor)));
  ShapeData graviton = polyCreate({Config::graviton, 400, scene.data_points});
  scene.shapes.add(new Shape(graviton));
  while (running)
  {
    scene.tick();
    scene.render();
    controls.handle();
  }
}