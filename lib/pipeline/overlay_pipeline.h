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
    glm::vec2 pos;
    glm::vec2 scale;
  };

public:
  OverlayPipeline(Context &ctx);
  void render(Scene &scene);
};