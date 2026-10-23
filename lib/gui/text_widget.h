#pragma once

#include "lib/gpu/gpu_stored_object.h"
#include "lib/gui/widget.h"
#include "lib/gpu/gpu_transferable.h"
#include "lib/gpu/gpu_stored_object.h"
#include "lib/pipeline/overlay/render_objects/text_overlay_render_object.h"
#include "lib/render/window.h"

class TextWidget : public Widget<TextOverlayRenderObject>, public GPUTransferable
{
  std::string content;
  GPUStoredObject gpu_content_handler;
  std::pair<Uint32, Uint32> glyph_size;
public:
  TextWidget(Window &window, std::pair<Uint32, Uint32> glyph_size);
  void setPosition(uint x, uint y);
  void setText(const std::string &content);
private:
  void updateWindowSize(uint w, uint h);
  void upload(SDL_GPUCopyPass *copy_pass) override;
};