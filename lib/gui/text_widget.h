#pragma once

#include "lib/gui/widget.h"
#include "lib/pbr/gpu_stored_object.h"
#include "lib/pipeline/overlay/render_objects/text_overlay_render_object.h"
#include "lib/pipeline/overlay/text_overlay_pipeline.h"
#include "lib/render/window.h"

class TextWidget : public Widget<TextOverlayRenderObject, TextOverlayPipeline>, public GPUStoredObject
{
  std::string content;
public:
  TextWidget(Window &window, TextOverlayPipeline& pipeline);
private:
  void updateWindowSize(uint w, uint h);
  void setText(const std::string &content);
  void upload(SDL_GPUCopyPass *copy_pass) override;
};