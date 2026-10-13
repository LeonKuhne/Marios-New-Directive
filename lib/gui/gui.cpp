#include "gui.h"
#include "lib/gui/fps_display.h"

GUI::GUI(Window &window)
{
  widgets.emplace_back(new FPSDisplay(window.ctx));
}

void GUI::render(Scene &scene, SDL_GPURenderPass *pass)
{
  for (Widget* widget : widgets)
  {
    widget->render(scene);
  }
}