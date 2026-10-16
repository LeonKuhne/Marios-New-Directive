#pragma once

#include <SDL3/SDL_stdinc.h>

class OverlayRenderObjectBase {
public:
  struct {
    struct {
      Uint32 width;
      Uint32 height;
      Uint32 pad[2];
    } window_size;
  } shared;

  struct {
    struct {
      Uint32 x;
      Uint32 y;
    } pos;
    struct {
      Uint32 width;
      Uint32 height;
    } scale;
  } vert;

  struct {
  } frag;

  struct {
  } buffers;
};