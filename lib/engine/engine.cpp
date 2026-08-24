#include "engine.h"

void Engine::run()
{
  SDL_Log("Running");

#if DRAW_GIZMOS
  // create gizmo
  scene.shapes.gizmo = new Shape(scene.data_points.finishCube(Config::gizmo));
#endif

  // add floor
  //scene.shapes.add(new Shape(scene.data_points.finishCube(Config::floor)));
  ShapeData graviton = polyCreate({Config::graviton, 400, scene.data_points});
  scene.shapes.add(new Shape(graviton));

#ifdef DRAW_GIZMOS
  // clear gizmos
  controls.mouse.onDragEnd.push_back([this]() { scene.shapes.gizmo_positions.clear(); });
#endif
  while (running)
  {
    scene.tick();
    scene.render();
    controls.handle();
  }
}