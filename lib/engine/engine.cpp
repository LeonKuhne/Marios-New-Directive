#include "engine.h"

void Engine::run()
{
  std::thread render_thread([&] {
    while (running)
      scene.render();
  });

  while (running)
  {
    ZoneScopedN("Game Loop");
    scene.tick();
    controls.handle();
  }

  render_thread.join();
}