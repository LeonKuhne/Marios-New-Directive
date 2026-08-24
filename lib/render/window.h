#pragma once

#include <sys/types.h>
#include <SDL3/SDL_gpu.h>
#include <vector>
#include <functional>
#include "lib/render/context.h"

class Window 
{
  std::vector<std::function<void(uint, uint)>> resize_listeners;

public:
  Context &ctx;
  uint width = 0;
  uint height = 0;

  Window(Context &context) : ctx(context) {resize(1, 1);};

  void resize(uint w, uint h);
  void addListener(std::function<void(uint, uint)> onResize) {
    resize_listeners.push_back(onResize);
  }
};