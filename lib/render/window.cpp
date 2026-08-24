#include "window.h"

void Window::resize(uint w, uint h)
{
  width = w;
  height = h;
  for (auto &listener : resize_listeners)
    listener(w, h);
}