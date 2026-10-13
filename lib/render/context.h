#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <btBulletDynamicsCommon.h>
#include <SDL3_ttf/SDL_ttf.h>

class Context
{
public:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_GPUDevice *gpu;
  btDiscreteDynamicsWorld* world;
  TTF_Font* font;

  Context();
  ~Context();

private:
  enum ErrorCode : uint8_t
  {
    SUCCESS = 0,
    INIT_ERROR = 1,
    WINDOW_ERROR = 2,
    RENDERER_ERROR = 3,
    GPU_ERROR = 4,
    TTF_ERROR = 5
  };

  ErrorCode initSDL();
  void initBullet();
};