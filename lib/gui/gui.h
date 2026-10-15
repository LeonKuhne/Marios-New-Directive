#pragma once

#include "lib/gui/text_widget.h"
#include "lib/render/context.h"
#include "lib/pipeline/overlay/overlay_pipeline.h"
#include "lib/pipeline/overlay/render_objects/text_overlay_render_object.h"
#include <SDL3/SDL_gpu.h>

class GUI
{
  OverlayPipeline<TextOverlayRenderObject> text_pipeline;
  TextWidget fps_display;

public:
  GUI(Context &ctx);
  void render(SDL_GPUCommandBuffer *cmd, SDL_GPURenderPass *pass);
};