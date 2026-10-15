#include "text_widget.h"
#include "lib/gpu/transfer.h"
#include <SDL3/SDL_surface.h>
#include <stdexcept>
#include <vector>
#include "lib/pipeline/overlay/render_objects/text_overlay_render_object.h"
#include <algorithm>
#include <cstring>

TextWidget::TextWidget(Context &ctx, OverlayPipeline<TextOverlayRenderObject>& pipeline) : Widget(pipeline)
{
  SDL_Color white = {
    .r = 255,
    .g = 255,
    .b = 255,
    .a = 255
  };

  std::vector<SDL_Surface*> glyphs;

  int atlas_width = 0;
  int atlas_height = 0;

  for (char c = 32; c <= 126; ++c)
  {
    SDL_Surface *glyph =
      TTF_RenderGlyph_Blended(ctx.font, c, white);

    if (!glyph)
      throw std::runtime_error(SDL_GetError());

    glyphs.push_back(glyph);

    atlas_width += glyph->w;
    atlas_height = std::max(atlas_height, glyph->h);
  }

  SDL_Surface *atlas = SDL_CreateSurface(
    atlas_width,
    atlas_height,
    SDL_PIXELFORMAT_RGBA8888
  );

  if (!atlas)
    throw std::runtime_error(SDL_GetError());

  int x = 0;

  for (SDL_Surface *glyph : glyphs)
  {
    SDL_Rect dst{
      .x = x,
      .y = 0,
      .w = glyph->w,
      .h = glyph->h
    };
    SDL_BlitSurface(glyph, nullptr, atlas, &dst);
    x += glyph->w;
  }

  for (SDL_Surface *glyph : glyphs)
  {
    SDL_DestroySurface(glyph);
  }

  transfer(ctx.gpu, [&](SDL_GPUCopyPass *pass) {
    atlas_texture = textureToGPU({
      .gpu = ctx.gpu,
      .pass = pass,
      .data = atlas->pixels,
      .width = static_cast<Uint32>(atlas->w),
      .height = static_cast<Uint32>(atlas->h)
    });
  });

  SDL_DestroySurface(atlas);
  memcpy(render_data.frag.text, "helowrld", 8);
  render_data.vert.pos[0] = 0.0f;
  render_data.vert.pos[1] = 0.1f;
  render_data.vert.scale[0] = 0.1f;
  render_data.vert.scale[1] = 0.1f;
};