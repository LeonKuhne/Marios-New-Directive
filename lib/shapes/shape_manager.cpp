#include "shape_manager.h"
#include <algorithm>

ShapeManager::~ShapeManager()
{
  for (Shape *shape : shapes)
    delete shape;
}

void ShapeManager::add(Shape *shape)
{
  shape->assignToWorld(ctx.world);
  shapes.push_back(shape);
}

void ShapeManager::add(ShapeData& data)
{
  Shape *shape = new Shape(data);
  shape->assignToWorld(ctx.world);
  shapes.push_back(shape);
}

void ShapeManager::remove(Shape *shape)
{
  auto it = std::find(shapes.begin(), shapes.end(), shape);
  if (it == shapes.end())
    return;

  if (shape->body)
  {
    ctx.world->removeRigidBody(shape->body);
    shape->body->setCollisionFlags(shape->body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    shape->body->setActivationState(DISABLE_DEACTIVATION);
  }

  if (selected == shape)
    selected = nullptr;

  shapes.erase(it);
  delete shape;
}

void ShapeManager::select(Shape *shape)
{
  // deselect previous
  if (selected)
  {
    selected->selected = false;
    selected = nullptr;
  }

  // select shape
  shape->selected = true;
  selected = shape;
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


void ShapeManager::render(Frame &frame, SDL_GPURenderPass *render_pass)
{
  pbr_pipeline.startRender(render_pass);
  // render frame
  for (Shape *shape : shapes)
  {
    pbr_pipeline.render(frame, shape);
  }
}