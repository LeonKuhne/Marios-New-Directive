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
      room_manager(ctx),
      collision_handler(player, room_manager),

      // game state
      running(running)
{
  collision_handler.setActiveRoomsCallback([this](const std::vector<Room*>& rooms) {
    active_rooms = rooms;
    if (active_rooms_changed)
      active_rooms_changed(active_rooms);
  });

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

}

void Scene::setActiveRoom(Room& room)
{
  collision_handler.setCurrentRoom(room);
  if (active_rooms.size() == 1 && active_rooms.front() == &room)
    return;

  active_rooms.clear();
  active_rooms.emplace_back(&room);
  if (active_rooms_changed)
    active_rooms_changed(active_rooms);
}

void Scene::setup(Mouse &mouse)
{
  // setup render passes
  frame.addPass([this, &mouse](Frame &frame, SDL_GPURenderPass *pass)
    {
      pbr_pipeline.startRender(pass);

      for (Room* room : active_rooms)
        room->render(*this, pass);
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
