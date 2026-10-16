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
  struct {
    struct {
      Uint32 width;
      Uint32 height;
    } glyph;
    Uint32 atlas_width;
    Uint32 _pad;
  } atlas_details;

  TextOverlayPipeline(Context &ctx) 
    : OverlayPipeline<TextOverlayRenderObject>(ctx, {
      .vertex_path = "rect.vert",
      .fragment_path = "text.frag",
      .has_vertex_input = false,
      .num_vert_uniforms = 0,
      .num_vert_storage_buffers = 0,
      .num_frag_uniforms = 1, // atlas size
      .num_frag_samplers = 1, // font atlas
      .num_frag_storage_buffers = 1, // text
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

    struct {
      int width = 0;
      int height = 0;
    } max_glyph_size;

    for (char c = 32; c <= 126; ++c)
    {
      SDL_Surface *glyph =
        TTF_RenderGlyph_Blended(ctx.font, c, white);

      if (!glyph)
        throw std::runtime_error(SDL_GetError());

      glyphs.push_back(glyph);
      max_glyph_size.width = std::max(glyph->w, max_glyph_size.width);
      max_glyph_size.height = std::max(glyph->h, max_glyph_size.height);
    }

    SDL_Surface *atlas = SDL_CreateSurface(
      max_glyph_size.width * static_cast<int>(glyphs.size()),
      max_glyph_size.height,
      SDL_PIXELFORMAT_RGBA8888
    );

    if (!atlas)
      throw std::runtime_error(SDL_GetError());

    atlas_details.glyph.width = max_glyph_size.width;
    atlas_details.glyph.height = max_glyph_size.height;
    atlas_details.atlas_width = atlas->w;

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
      x += max_glyph_size.width;
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

    for (SDL_Surface *glyph : glyphs)
      SDL_DestroySurface(glyph);
    SDL_DestroySurface(atlas);
  };

  void render(SDL_GPUCommandBuffer *cmd, TextOverlayRenderObject &render_data) override {
    // pipeline data
    SDL_PushGPUFragmentUniformData(cmd, 2, &atlas_details, sizeof(atlas_details));

    // atlas sampler
    SDL_BindGPUFragmentSamplers(this->render_pass, 0, &atlas_binding, 1);

    // text buffer
    SDL_BindGPUFragmentStorageBuffers(render_pass, 0, &render_data.buffers.text, 1);

    // overlay render
    OverlayPipeline<TextOverlayRenderObject>::render(cmd, render_data);
  };
};