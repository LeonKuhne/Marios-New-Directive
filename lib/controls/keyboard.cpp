#include "keyboard.h"

#include "lib/scene/scene.h"

void Keyboard::setup(Scene &scene)
{
  // exit (escape)
  bindKey(SDLK_ESCAPE, DOWN, [&]() { scene.running = false; });

  // spawn poly (page up)
  bindKey(SDLK_PAGEUP, HELD, [&]() { scene.spawnAsteroid(); });

  // jump player (space)
  bindKey(SDLK_SPACE, HELD, [&]() { scene.player.jump(); });

  // sprint (shift)
  bindKey(SDLK_LSHIFT, DOWN, [&]() { scene.player.setSprinting(true); });
  bindKey(SDLK_LSHIFT, UP, [&]() { scene.player.setSprinting(false); });
  bindKey(SDLK_RSHIFT, DOWN, [&]() { scene.player.setSprinting(true); });
  bindKey(SDLK_RSHIFT, UP, [&]() { scene.player.setSprinting(false); });

  // move player (esdf)
  bindKey(SDLK_PERIOD, HELD, [&]() { scene.player.move({1.0f, 0.0f}); });
  bindKey(SDLK_E, HELD, [&]() { scene.player.move({-1.0f, 0.0f}); });
  bindKey(SDLK_U, HELD, [&]() { scene.player.move({0.0f, 1.0f}); });
  bindKey(SDLK_O, HELD, [&]() { scene.player.move({0.0f, -1.0f}); });

  // adjust sim speed (<, >)
  bindKey(SDLK_W, HELD, [&]() { scene.sim_speed = std::max(0.0001f, scene.sim_speed * 0.95f); });
  bindKey(SDLK_V, HELD, [&]() { scene.sim_speed = std::min(1.0f, scene.sim_speed * 1.05f); });
}
