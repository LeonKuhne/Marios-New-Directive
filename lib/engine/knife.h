#pragma once

#include "lib/slicer/slicer.h"
#include "lib/controls/controls.h"
#include "lib/scene/scene.h"

class Knife
{
  Mouse &mouse;
  Window &window;
  ShapeManager &shapes;
  Camera &camera;
  Slicer slicer;
  int slice_callback_id = -1;

public:

  Knife(Controls &controls, Scene &scene)
      : mouse(controls.mouse),
        window(scene.window),
        shapes(scene.shapes),
        camera(scene.camera),
        slicer(scene.data_points)
  {}

  void toggle() 
  {
    if (isEnabled()) 
      disable();
    else 
      enable();
  }

  bool isEnabled() const 
  { 
    return slice_callback_id != -1; 
  }

  void slice(Shape *shape)
  {
    // slice shape
    Shape *slices[2];
    slicer.select(shapes.selected);
    slicer.align({
        camera.screenToWorld(mouse.start.x, mouse.start.y),
        camera.screenToWorld(mouse.pos.x, mouse.pos.y),
        camera.pos,
    });
    if (!slicer.slice(slices))
      return;

    // update shapes in scene
    for (int i = 0; i < 2; ++i)
      shapes.add(slices[i]);
    shapes.remove(shapes.selected);

    // select bigger shape
    Shape *bigger_shape = slices[0]->volume > slices[1]->volume ? slices[0] : slices[1];
    shapes.select(bigger_shape);
#ifdef DRAW_GIZMOS
    shapes.addVertexGizmos(bigger_shape);
#endif
  }

private:
  void enable()
  {
    camera.disable();

    // slice on drag
    slice_callback_id = mouse.onUp.add([this]()
                                            {
      if (shapes.selected != nullptr)
        slice(shapes.selected); });
  }
  void disable()
  {
    camera.enable();

    mouse.onUp.remove(slice_callback_id);
    slice_callback_id = -1;
  }
};