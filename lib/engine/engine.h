#pragma once

#include <SDL3/SDL.h>
#include "lib/scene/scene.h"
#include "lib/slicer/slicer.h"
#include "lib/mesh/poly.h"
#include "lib/controls/controls.h"
#include "knife.h"

class Engine
{
  Controls controls;
  Scene scene;
  Knife knife;
  bool running = false;

public:
  Engine() : scene(running, controls.mouse), knife(controls, scene) {
    scene.setup(knife, controls.mouse);
    controls.setup(scene, knife);
    running = true;
  }
  void run();
};