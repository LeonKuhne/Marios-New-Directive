#pragma once

#include "lib/gui/widget.h"
#include "lib/render/window.h"
#include <SDL3/SDL_gpu.h>

class GUI
{
  std::vector<Widget*> widgets;
public:
  GUI(Window &window);
  void render(Scene &scene, SDL_GPURenderPass *pass);
};