#pragma once

#include "lib/gui/text_widget.h"
#include "lib/pipeline/overlay/text_overlay_pipeline.h"
#include "lib/render/window.h"
#include <SDL3/SDL_gpu.h>

class GUI
{
  TextOverlayPipeline text_pipeline;

public:
  TextWidget render_fps_display;
  TextWidget tick_rate_display;

  GUI(Window &window);
  void render(SDL_GPUCommandBuffer *cmd, SDL_GPURenderPass *pass);
};