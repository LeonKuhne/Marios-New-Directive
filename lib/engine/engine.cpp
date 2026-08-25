#include "engine.h"
#include "lib/procgen/hallways.h"

void Engine::run()
{
  SDL_Log("Running");

  // hallways
  HallwayGenerator::generate(scene);

  while (running)
  {
    scene.tick();
    scene.render();
    controls.handle();
  }
}