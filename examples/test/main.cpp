#include "lib/engine/engine.h"

static inline const ShapeData FLOOR = {
    .collider_type = Config::Colliders::SURFACE,
    .pos = {0.0f, 0.0f, 0.0f},
    //.rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    .scale = {10.0f, 0.0f, 10.0f},
    .density = 1000.0f,
    .color = {0.0f, 0.0f, 1.0f, 1.0f},
};

int main() {
  Engine engine = Engine();
  Scene& scene = engine.getScene();

  ShapeData floor = FLOOR;
  scene.plane_builder.build(floor);
  Room& room = scene.room_manager.createRoom();
  room.addSurface(floor);
  scene.room_manager.setActive(room);

  scene.light_manager.add(Light{.pos = glm::vec3(0.0f, 1.0f, 0.0f), .intensity = 5000.0f});
  scene.light_manager.updateLights();

  engine.run();
  return EXIT_SUCCESS;
}