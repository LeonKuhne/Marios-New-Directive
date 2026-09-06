#include "engine.h"

void Engine::run()
{
  while (running)
  {
    scene.tick();
    scene.render();
    controls.handle();
  }
}