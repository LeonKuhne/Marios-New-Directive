#pragma once

#include "lib/gui/text_widget.h"
#include "lib/pipeline/overlay/text_overlay_pipeline.h"
#include "lib/render/context.h"
#include <SDL3/SDL_gpu.h>

class GUI
{
  TextOverlayPipeline text_pipeline;
  TextWidget fps_display;

public:
  GUI(Context &ctx);
  void render(SDL_GPUCommandBuffer *cmd, SDL_GPURenderPass *pass);
};