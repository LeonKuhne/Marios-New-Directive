#include "engine.h"

void Engine::run()
{
  while (running)
  {
    ZoneScopedN("Game Loop");
    scene.tick();
    scene.render();
    controls.handle();
  }
}