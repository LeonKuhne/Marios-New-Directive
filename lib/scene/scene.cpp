#include "scene.h"
#include "lib/gpu/gpu_transferable.h"
#include "lib/engine/config.h"
#include <tracy/Tracy.hpp>
#include "lib/gpu/transfer.h"

Scene::Scene(bool &running, Mouse &mouse)
    : window(Window(ctx)),

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

      // render pipelines
      pbr_pipeline(PBRPipeline({
        .ctx=ctx, 
        .camera=camera, 
        .light_manager=light_manager, 
        .pbr_vertices=pbr_vertices, 
        .pbr_materials=pbr_materials
      })),

      // render composites
      gui(GUI(window)),
      room_manager(ctx, pbr_pipeline),

      // game state
      running(running)
{
  updateGPUData();

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

void Scene::setup(Mouse &mouse)
{
  // setup pbr passes
  frame.addPass([this, &mouse](SDL_GPUCommandBuffer *cmd, SDL_GPURenderPass *pass)
    {
      room_manager.render(cmd, pass);
      gui.render(cmd, pass);
    }
  );
}

void Scene::tick()
{
  ZoneScoped;

  double tick_delta;
  {
    ZoneScopedN("Track Tick Rate");
    Uint64 current_tick_time = SDL_GetPerformanceCounter();
    tick_delta = static_cast<double>(current_tick_time - last_tick_time) / performance_frequency;
    last_tick_time = current_tick_time;
    double fps = 1.0 / tick_delta;
    std::string fps_content = std::format("tick rate: {:5}", round(fps));
    gui.tick_rate_display.setText(fps_content);
    TracyPlot("tick_rate", 1.0f / tick_delta);
  }

  {
    ZoneScopedN("Step Simulation");
    ctx.world->stepSimulation(static_cast<float>(tick_delta), 5, 1.0f / 60.0f);
  }
  {
    ZoneScopedN("Player Tick");
    player.tick();
  }
  {
    ZoneScopedN("Camera Tick");
    camera.tick();
  }
}

void Scene::render()
{
  ZoneScoped;

  // aquire next frame
  {
    ZoneScopedN("Render Frame");
    if(!frame.load())
      return;
  }

  {
    ZoneScopedN("Track Render FPS");
    Uint64 current_render_time = SDL_GetPerformanceCounter();
    double render_delta = static_cast<double>(current_render_time - last_render_time) / performance_frequency;
    last_render_time = current_render_time;

    double fps = 1.0 / render_delta;
    std::string fps_content = std::format("render fps: {:5}", round(fps));
    gui.render_fps_display.setText(fps_content);
    TracyPlot("render_fps", fps);
  }

  {
    ZoneScopedN("GPU Transfer");
    updateGPUData();
  }

  {
    ZoneScopedN("Render Frame");
    frame.publish();
  }
}

void Scene::gravityTick(btScalar timeStep)
{
  constexpr btScalar gravity_strength = 9.8f;
  const btVector3 gravity_dir(0.0f, -1.0f, 0.0f);

  btScalar gravity = gravity_strength;
  player.body->setGravity(gravity_dir * gravity);
}

void Scene::updateGPUData()
{
  // upload pending updates
  transfer(ctx.gpu, [this](SDL_GPUCopyPass *pass) {
    GPUTransferable::processPendingUpdates(pass);
  });
}