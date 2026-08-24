#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <btBulletDynamicsCommon.h>
#include "data_points.h"

class Context
{
public:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_GPUDevice *gpu;
  btDiscreteDynamicsWorld* world;

  Context();
  ~Context();

private:
  enum ErrorCode
  {
    SUCCESS = 0,
    INIT_ERROR = 1,
    WINDOW_ERROR = 2,
    RENDERER_ERROR = 3,
    GPU_ERROR = 4
  };

  ErrorCode initSDL();
  void initBullet();
};