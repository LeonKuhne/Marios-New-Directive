#include "shape_manager.h"
#include "lib/scene/scene.h"
#include <algorithm>

ShapeManager::~ShapeManager()
{
  for (Shape *shape : shapes)
    delete shape;
  for (Solid *solid : visible_shapes)
    delete solid;
}

void ShapeManager::add(Shape *shape)
{
  shapes.push_back(shape);
}

void ShapeManager::add(Solid *solid)
{
  solid->assignToWorld(ctx.world);
  add(static_cast<Shape*>(solid));
  if (solid->is_visible)
    visible_shapes.push_back(solid);
}

Shape* ShapeManager::add(ShapeData& data)
{
  Shape *shape = new Shape(data);
  add(shape);
  return shape;
}

void ShapeManager::remove(Shape *shape)
{
  auto it = std::find(shapes.begin(), shapes.end(), shape);
  if (it == shapes.end())
    return;
  shapes.erase(it);
  delete shape;
}

/*
void ShapeManager::updateRenderVars(Frame &frame, SDL_GPUCopyPass *copy_pass)
{
  // dequeue updates
  for (Shape *shape : shapes)
  {
    pbr_pipeline.pushVars();
  }
}
*/

void ShapeManager::render(Scene& scene, SDL_GPURenderPass *render_pass)
{
  for (Solid *solid : visible_shapes)
  {
    scene.pbr_pipeline.render(scene, solid);
  }
}