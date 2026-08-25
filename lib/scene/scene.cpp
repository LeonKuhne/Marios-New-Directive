#include "scene.h"

#include "lib/engine/config.h"
#include <iostream>

Scene::Scene(bool &running, Mouse &mouse)
    : window(Window(ctx)),
      player(Player({
        .pos = Config::PlayerSettings::spawn_pos,
      })),
      camera(Camera(window, player, mouse)),
      data_points(ctx.gpu),
      frame(Frame(window, camera, data_points.vertex_buffer)),
      shapes(ShapeManager(ctx)),
      running(running)
{
  // setup gravity
  ctx.world->setInternalTickCallback([](btDynamicsWorld *world, btScalar timeStep)
    {
      // Retrieve your Scene (or whatever data you need) from userInfo
      Scene *scene = static_cast<Scene *>(world->getWorldUserInfo());
      scene->gravityTick(timeStep); 
    }, static_cast<void *>(this) // <-- passed as worldUserInfo
  );

  // handle graviton collisions
  static Scene *scene_instance = this;
  gContactStartedCallback = [](btPersistentManifold *const &manifold)
  {
    scene_instance->checkCollision(manifold);
  };

  ctx.world->addRigidBody(player.body);
}

void Scene::setup(Mouse &mouse)
{
  // setup render passes
  frame.addPass([this, &mouse](Frame &frame, SDL_GPURenderPass *pass)
    {
      shapes.render(frame, pass);
    }
  );
}

void Scene::tick()
{
  const float fps = 60.0f;
  const float delta = 1.0f / fps;
  ctx.world->stepSimulation(delta * sim_speed, 1);
  player.tick();
  camera.tick();
  ticks++;
}

void Scene::gravityTick(btScalar timeStep)
{
  constexpr btScalar gravity_strength = 9.8f;
  const btVector3 gravity_dir(0.0f, -1.0f, 0.0f);

  for (Shape *shape : shapes.shapes)
  {
    if (shape->is_static || !shape->body)
      continue;
    shape->body->setGravity(gravity_dir * gravity_strength);
  }

  btScalar gravity = gravity_strength;
  player.body->setGravity(gravity_dir * gravity);
}

void Scene::checkCollision(btPersistentManifold *const &manifold)
{
  // get the two colliding bodies
  const btCollisionObject *body_a = manifold->getBody0();
  const btCollisionObject *body_b = manifold->getBody1();
  ShapeBase *shape_a = static_cast<ShapeBase *>(body_a->getUserPointer());
  ShapeBase *shape_b = static_cast<ShapeBase *>(body_b->getUserPointer());

  if (!shape_a || !shape_b)
    return;

  ushort type_a = shape_a->getType();
  ushort type_b = shape_b->getType();

  // Graviton collision.
  if (type_a == ShapeType::GRAVITON || type_b == ShapeType::GRAVITON)
  {
    if (type_a == ShapeType::GRAVITON)
    {
      if (type_b == ShapeType::ASTEROID)
        shapes.remove(static_cast<Shape *>(shape_b));
    }
    else
    {
      if (type_a == ShapeType::ASTEROID)
        shapes.remove(static_cast<Shape *>(shape_a));
    }
  }

  // Player grounding.
  if (body_a == player.body || body_b == player.body)
  {
    const btCollisionObject *other = body_a == player.body ? body_b : body_a;

    for (int i = 0; i < manifold->getNumContacts(); ++i)
    {
      const btManifoldPoint &contact = manifold->getContactPoint(i);

      if (contact.getDistance() > 0.05f)
        continue;

      btVector3 normal = contact.m_normalWorldOnB;

      if (other == body_a)
        normal = -normal;

      // Player's local "up" is away from the planet.
      btVector3 player_pos =
        player.body->getCenterOfMassPosition();

      btVector3 player_up = player_pos.normalized();

      // Contact is floor if its normal points approximately upward.
      if (normal.dot(player_up) > 0.5f)
      {
        player.isGrounded = true;

        btVector3 velocity = player.body->getLinearVelocity();

        // Remove velocity into/out of the floor.
        velocity -= normal * velocity.dot(normal);

        player.body->setLinearVelocity(velocity);
        break;
      }
    }
  }
}