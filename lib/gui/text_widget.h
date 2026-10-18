#pragma once

#include "lib/gpu/gpu_stored_object.h"
#include "lib/gui/widget.h"
#include "lib/gpu/gpu_transferable.h"
#include "lib/gpu/gpu_stored_object.h"
#include "lib/pipeline/overlay/render_objects/text_overlay_render_object.h"
#include "lib/pipeline/overlay/text_overlay_pipeline.h"
#include "lib/render/window.h"

class TextWidget : public Widget<TextOverlayRenderObject, TextOverlayPipeline>, public GPUTransferable
{
  std::string content;
  GPUStoredObject gpu_content_handler;
public:
  TextWidget(Window &window, TextOverlayPipeline& pipeline);
  void setPosition(uint x, uint y);
  void setText(const std::string &content);
private:
  void updateWindowSize(uint w, uint h);
  void upload(SDL_GPUCopyPass *copy_pass) override;
};