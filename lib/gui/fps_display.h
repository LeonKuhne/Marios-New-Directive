#pragma once

#include "lib/gui/widget.h"
#include "lib/render/context.h"
#include <unordered_map>

class FPSDisplay : public Widget
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
  FPSDisplay(Context &ctx);
  void render(Scene &scene) override;
};