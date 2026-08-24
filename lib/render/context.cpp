#include "context.h"

Context::Context()
{
  ErrorCode code = initSDL();
  if (code != SUCCESS)
    SDL_Log("SDL Error (%d): %s\n", code, SDL_GetError());

  initBullet();
}

Context::~Context()
{
  if (gpu)
    SDL_DestroyGPUDevice(gpu);
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
}

Context::ErrorCode Context::initSDL()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
    return INIT_ERROR;

  window = SDL_CreateWindow("Engine", 800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
  if (window == NULL)
    return WINDOW_ERROR;

  renderer = SDL_CreateRenderer(window, NULL);
  if (renderer == NULL)
    return RENDERER_ERROR;

  gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, NULL);
  if (!gpu)
    return GPU_ERROR;
  if (!SDL_ClaimWindowForGPUDevice(gpu, window))
    return GPU_ERROR;

  return SUCCESS;
}

void Context::initBullet() 
{
  btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
  btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
  btBroadphaseInterface* broadphase = new btDbvtBroadphase();
  btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
  world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
  world->setGravity(btVector3(0, 0, 0));
}