#include "engine.h"
#include "lib/procgen/hallways.h"

void Engine::run()
{
  SDL_Log("Running");

  // hallways
  HallwayGenerator::generate(scene, glm::vec3(0.0f, 0.0f, 0.0f));

  while (running)
  {
    scene.tick();
    scene.render();
    controls.handle();
  }
}