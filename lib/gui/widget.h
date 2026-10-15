#pragma once

#include "lib/pipeline/overlay/overlay_pipeline.h"
#include "lib/pipeline/overlay/render_objects/overlay_render_object_base.h"
#include <SDL3/SDL_gpu.h>
#include <concepts>

template<std::derived_from<OverlayRenderObjectBase> T>
class Widget
{
protected:
  OverlayPipeline<T> &pipeline;
  T render_data;

public:
  Widget(OverlayPipeline<T> &pipeline) : pipeline(pipeline) {};

  void render(SDL_GPUCommandBuffer *cmd) {
    pipeline.render(cmd, render_data);
  };
};