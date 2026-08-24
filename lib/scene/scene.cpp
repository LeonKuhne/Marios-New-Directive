#include "scene.h"

#include "lib/engine/config.h"

Scene::Scene(bool &running, Mouse &mouse)
    : window(Window(ctx)),
      player(Player({
        .pos = Config::player_spawn_pos,
        .linear_damping = Config::player_air_friction,
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
  float fps = 60.0f;
  float delta = 1.0f / fps;
  ctx.world->stepSimulation(delta * sim_speed, 1);
  camera.update();

  // spawn an asteroid every spawn interval ticks
  if (ticks % Config::asteroid_tick_spawn_interval == 0)
    if (shapes.shapes.size() < Config::num_asteroids)
    {
      spawnAsteroid();
      SDL_Log("Spawned asteroid, total shapes: %zu", shapes.shapes.size());
    }

  ticks++;
}

Shape *Scene::spawnAsteroid()
{
  ShapeData shape_data = polyCreate({Config::asteroid, 20, data_points});
  shape_data.linear_velocity = random_vertex() * Config::asteroid_spawn_velocity;
  shape_data.pos = glm::normalize(random_vertex()) * Config::asteroid_spawn_distance;
  Shape *shape = new Shape(shape_data);
  shapes.add(shape);
  shapes.select(shape);
  return shape;
}

void Scene::gravityTick(btScalar timeStep)
{
  static float angle = 0.0f;
  angle += timeStep * 0.5f; // rotate gravity direction over time
  float strength = 9.8f;

  // attract to 0, 0
  for (Shape *shape : shapes.shapes)
  {
    if (shape->is_static)
      continue;

    btRigidBody *body = shape->body;
    if (!body)
      continue;

    // respawn asteroid if inactive
    /*
    if (!(body->isActive()))
    {
      shapes.remove(shape);
      spawnAsteroid();
    }
    */

    // simulate asteroid physics
    btVector3 pos = body->getCenterOfMassPosition();
    btVector3 gravity_dir = -pos.normalized(); // direction towards the center
    body->setGravity(gravity_dir * strength);
  }

  // simulate physics on player
  btVector3 player_pos = player.body->getCenterOfMassPosition();
  btVector3 player_gravity_dir = -player_pos.normalized();
  float player_gravity_factor = player.isGrounded ? Config::player_ground_gravity_factor : 1.0f;
  player.body->setGravity(player_gravity_dir * strength * player_gravity_factor);
}

void Scene::checkCollision(btPersistentManifold *const &manifold)
{
  // get the two colliding bodies
  ShapeBase *shape_a = static_cast<ShapeBase *>(manifold->getBody0()->getUserPointer());
  ShapeBase *shape_b = static_cast<ShapeBase *>(manifold->getBody1()->getUserPointer());
  ushort type_a = shape_a->getType();
  ushort type_b = shape_b->getType();

  // check if one graviton
  if (!(type_a == ShapeType::GRAVITON xor type_b == ShapeType::GRAVITON))
    return;

  // reorder so that shapeA is the graviton
  ShapeBase *grav = type_a == ShapeType::GRAVITON ? shape_a : shape_b;
  ShapeBase *other = grav == shape_a ? shape_b : shape_a;

  // despawn asteroid
  if (other->getType() == ShapeType::ASTEROID)
    shapes.remove(static_cast<Shape *>(other));
}
