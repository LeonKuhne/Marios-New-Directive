#pragma once

#include "lib/pipeline/overlay/overlay_pipeline.h"
#include "lib/pipeline/overlay/render_objects/text_overlay_render_object.h"
#include "lib/render/context.h"
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_stdinc.h>
#include <glm/ext/vector_float2.hpp>
#include <stdexcept>

class Scene;

class TextOverlayPipeline : public OverlayPipeline<TextOverlayRenderObject> {
private:
  SDL_GPUTextureSamplerBinding atlas_binding;

public:
  TextOverlayPipeline(Context &ctx) 
    : OverlayPipeline<TextOverlayRenderObject>(ctx, {
      .vertex_path = "rect.vert",
      .fragment_path = "text.frag",
      .has_vertex_input = false,
      .num_vert_uniforms = 1,
      .num_vert_storage_buffers = 0,
      .num_frag_uniforms = 0,
      .num_frag_samplers = 1, // font atlas
      .num_frag_storage_buffers = 0,
      .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
      .enable_depth_test = false,
      .enable_depth_write = false
    })
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
      atlas_binding.sampler = ctx.sampler;
      atlas_binding.texture = textureToGPU({
        .gpu = ctx.gpu,
        .pass = pass,
        .data = atlas->pixels,
        .width = static_cast<Uint32>(atlas->w),
        .height = static_cast<Uint32>(atlas->h)
      });
    });

    SDL_DestroySurface(atlas);
  };

  void render(SDL_GPUCommandBuffer *cmd, TextOverlayRenderObject &render_data) override {
    SDL_BindGPUFragmentSamplers(this->render_pass, 0, &atlas_binding, 1);
    OverlayPipeline<TextOverlayRenderObject>::render(cmd, render_data);
  };
};