#pragma once

#include <functional>
#include <map>
#include "lib/util/callback_list.h"

class Mouse
{
  struct Position
  {
    int x, y;
  };

  typedef std::function<void(Mouse)> MouseCallback;

public:
  Position pos, start, end, delta;
  bool dragging = false;
  CallbackList onDown;
  CallbackList onMove;
  CallbackList onUp;

  void down(int x, int y)
  {
    dragging = true;
    entry(x, y, start, onDown);
  }

  void move(int x, int y, int dx, int dy)
  {
    delta = {dx, dy};
    entry(x, y, pos, onMove);
  }

  void up(int x, int y)
  {
    dragging = false;
    entry(x, y, end, onUp);
  }

private:
  void entry(int x, int y, Position &pos, CallbackList &callbacks)
  {
    pos = {x, y};
    callbacks.runAll();
  }
};