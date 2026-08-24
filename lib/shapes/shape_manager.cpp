#include "shape_manager.h"

ShapeManager::~ShapeManager()
{
  for (Shape *shape : shapes)
    delete shape;
#ifdef DRAW_GIZMOS
  delete gizmo;
#endif
}

void ShapeManager::add(Shape *shape)
{
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

#ifdef DRAW_GIZMOS
void ShapeManager::addVertexGizmos(Shape *shape)
{
  btCollisionShape *collisionShape = shape->body->getCollisionShape();
  // ignore if no collider
  if (!collisionShape)
    return;

  // render gizmos at convex hull vertices
  btConvexHullShape *convex = (btConvexHullShape *)collisionShape;
  int num_points = convex->getNumPoints();
  for (int v = 0; v < num_points; v++)
  {
    addGizmo([convex, shape, v]()
             {
        btVector3 origin = shape->body->getWorldTransform().getOrigin();
        btTransform world_transform = shape->body->getWorldTransform();
        btVector3 vertex; convex->getVertex(v, vertex);
        return origin + world_transform.getBasis() * vertex; });
  }
}
#endif

void ShapeManager::render(Frame &frame, SDL_GPURenderPass *pass)
{
  shape_pipeline.start(pass);

  for (Shape *shape : shapes)
  {
    shape_pipeline.render(frame, shape);
  }

#ifdef DRAW_GIZMOS
  for (auto &getPosition : gizmo_positions)
  {
    gizmo->setPosition(getPosition());
    shape_pipeline.render(frame, gizmo);
  }
#endif
}