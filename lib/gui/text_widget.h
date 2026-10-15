#pragma once

#include "lib/gui/widget.h"
#include "lib/pipeline/overlay/overlay_pipeline.h"
#include "lib/pipeline/overlay/render_objects/text_overlay_render_object.h"
#include "lib/render/context.h"
#include <unordered_map>

class TextWidget : public Widget<TextOverlayRenderObject>
{
private:
  struct Glyph
  {
      float u0, v0;
      float u1, v1;
      float width;
      float height;
  };

  SDL_GPUTexture *atlas_texture;
  std::unordered_map<char, Glyph> glyphs;

public:
  TextWidget(Context &ctx, OverlayPipeline<TextOverlayRenderObject>& pipeline);
};