#pragma once

#include <SDL3/SDL_log.h>
#include <functional>
#include "lib/util/callback_list.h"

class Mouse
{
  struct Position
  {
    float x, y;
  };

  typedef std::function<void(Mouse)> MouseCallback;

public:
  Position pos, start, end, delta;
  bool dragging = false;
  CallbackList onDown;
  CallbackList onMove;
  CallbackList onUp;

  void down(float x, float y)
  {
    dragging = true;
    entry(x, y, start, onDown);
  }

  void move(float x, float y, float dx, float dy)
  {
    delta = {dx, dy};
    entry(x, y, pos, onMove);
  }

  void up(float x, float y)
  {
    dragging = false;
    entry(x, y, end, onUp);
  }

private:
  void entry(float x, float y, Position &pos, CallbackList &callbacks)
  {
    pos = {x, y};
    callbacks.runAll();
  }
};