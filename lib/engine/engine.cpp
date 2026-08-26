#include "engine.h"
#include "lib/procgen/hallways.h"
#include "lib/debug/timer.h"

void Engine::run()
{
  SDL_Log("Running");

  HallwayGenerator::generate(scene);

  while (running)
  {
    scene.tick();
    scene.render();
    controls.handle();
  }
}