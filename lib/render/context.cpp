#include "context.h"
#include <SDL3/SDL_gpu.h>

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
  if (font)
    TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
}

Context::ErrorCode Context::initSDL()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
    return INIT_ERROR;

  window = SDL_CreateWindow("Engine", 800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
  if (window == nullptr)
    return WINDOW_ERROR;

  renderer = SDL_CreateRenderer(window, nullptr);
  if (renderer == nullptr)
    return RENDERER_ERROR;

  gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, NULL);
  if (!gpu)
    return GPU_ERROR;
  if (!SDL_ClaimWindowForGPUDevice(gpu, window))
    return GPU_ERROR;


  if (!SDL_SetGPUSwapchainParameters(
    gpu, 
    window, 
    SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
    SDL_GPU_PRESENTMODE_IMMEDIATE
    //SDL_GPU_PRESENTMODE_VSYNC
  ))
    return SWAPCHAIN_ERROR;

  SDL_GPUSamplerCreateInfo sampler_info{};
  sampler_info.min_filter = SDL_GPU_FILTER_LINEAR;
  sampler_info.mag_filter = SDL_GPU_FILTER_LINEAR;
  sampler_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
  sampler_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
  sampler_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
  sampler_info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
  sampler = SDL_CreateGPUSampler(gpu, &sampler_info);
  if (!sampler)
    return SAMPLER_ERROR;

  if (!TTF_Init())
    return TTF_ERROR;
  font = TTF_OpenFont("fonts/RobotoMono-Medium.ttf", 16);
  if (!font)
    return TTF_ERROR;

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