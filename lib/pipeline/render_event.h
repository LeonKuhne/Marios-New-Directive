#pragma once

#include <SDL3/SDL_gpu.h>

template<typename>
class Pipeline;

template<typename T>
class RenderEvent
{
public:
  Pipeline<T>& pipeline;
  SDL_GPUCommandBuffer* cmd;
  SDL_GPURenderPass* pass;

  RenderEvent(Pipeline<T>& pipeline, SDL_GPUCommandBuffer* cmd, SDL_GPURenderPass *pass)
  : pipeline(pipeline), cmd(cmd), pass(pass)
  {
    SDL_BindGPUGraphicsPipeline(pass, pipeline.pipeline);
  }

  void render(T render_object) { pipeline.render(*this, render_object); }
};