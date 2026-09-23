#include "scene.h"
#include "lib/pbr/gpu_stored_object.h"
#include "lib/engine/config.h"
#include <tracy/Tracy.hpp>
#include "lib/gpu/transfer.h"

Scene::Scene(bool &running, Mouse &mouse)
    : window(Window(ctx)),
      pbr_pipeline(PBRPipeline(ctx)),

      // game objects
      player(Player({ .pos = Config::PlayerSettings::spawn_pos })),
      camera(Camera(window, player, mouse)),

      // gpu storage
      plane_builder(ctx.gpu, all_vertices),
      pbr_vertices(ctx.gpu, all_vertices),
      pbr_materials(ctx.gpu),

      // rendering
      light_manager(LightManager(ctx.gpu)),
      frame(Frame(window)),

      // game state
      running(running)
{
  static Scene *scene_instance = this;

  // upload pending updates
  transfer(ctx.gpu, [this](SDL_GPUCopyPass *pass) {
    GPUStoredObject::processPendingUpdates(pass);
  });

  // setup gravity
  ctx.world->setInternalTickCallback([](btDynamicsWorld *world, btScalar timeStep)
    {
      // Retrieve your Scene (or whatever data you need) from userInfo
      Scene *scene = static_cast<Scene *>(world->getWorldUserInfo());
      scene->gravityTick(timeStep); 
    }, static_cast<void *>(this) // <-- passed as worldUserInfo
  );

  ctx.world->addRigidBody(player.body);

  // handle collisions
  gContactStartedCallback = [](btPersistentManifold *const &manifold)
  {
    scene_instance->checkCollision(manifold);
  };
}

void Scene::setup(Mouse &mouse)
{
  // setup render passes
  frame.addPass([this, &mouse](Frame &frame, SDL_GPURenderPass *pass)
    {
      pbr_pipeline.startRender(pass);

      // render active rooms
      rooms[0].render(*this, pass);
    }
  );
}

void Scene::tick()
{
  ZoneScoped;
  Uint64 current_time = SDL_GetPerformanceCounter();
  float delta_time = static_cast<float>(current_time - last_game_time) / static_cast<float>(SDL_GetPerformanceFrequency());
  last_game_time = current_time;

  ctx.world->stepSimulation(delta_time, 5, 1.0f / 60.0f);
  player.tick();
  camera.tick();
}

void Scene::gravityTick(btScalar timeStep)
{
  constexpr btScalar gravity_strength = 9.8f;
  const btVector3 gravity_dir(0.0f, -1.0f, 0.0f);

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