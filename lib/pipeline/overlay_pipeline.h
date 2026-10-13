#pragma once

#include "pipeline.h"
#include "lib/render/context.h"
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_stdinc.h>
#include <glm/ext/vector_float2.hpp>

class Scene;

class OverlayPipeline : public Pipeline {
private:
  SDL_GPUGraphicsPipeline *pipeline;

  struct OverlayData {
    float pos[2];
    float scale[2];
  };

public:
  OverlayPipeline(Context &ctx);
  void render(Scene &scene);
};