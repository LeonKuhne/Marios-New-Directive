#pragma once

#include <SDL3/SDL.h>
#include "lib/scene/scene.h"
#include "lib/mesh/poly.h"
#include "lib/controls/controls.h"

class Engine
{
  Controls controls;
  Scene scene;
  bool running = false;

public:
  Engine() : scene(running, controls.mouse) {
    scene.setup(controls.mouse);
    controls.setup(scene);
    running = true;
  }
  void run();
};