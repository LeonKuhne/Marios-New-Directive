#pragma once

#include "lib/pipeline/overlay/render_objects/overlay_render_object_base.h"
#include "lib/pipeline/render_event.h"
#include <SDL3/SDL_gpu.h>
#include <concepts>

template<std::derived_from<OverlayRenderObjectBase> T>
class Widget
{
protected:
  T render_data;

public:
  void render(RenderEvent<T> &event) {
    event.render(render_data);
  };
};