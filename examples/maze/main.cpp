#include "lib/engine/engine.h"
#include "procgen/hallways.h"

int main() {
  Engine engine = Engine();
  HallwayGenerator hallwayGenerator;
  hallwayGenerator.generate(engine.getScene(), 580085);
  RoomManager& roomManager = engine.getScene().room_manager;
  roomManager.setActive(roomManager.find(0));
  hallwayGenerator.renderMap(roomManager.active_rooms);
  engine.run();
  return EXIT_SUCCESS;
}