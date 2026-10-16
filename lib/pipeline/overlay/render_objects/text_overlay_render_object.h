#pragma once

#include "overlay_render_object_base.h"
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_stdinc.h>

class TextOverlayRenderObject : public OverlayRenderObjectBase {
public:
  struct {
    Uint32 text_num_chars;
  } frag;

  struct {
    SDL_GPUBuffer *text;
  } buffers;
};