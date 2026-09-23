#include "lib/engine/engine.h"
#include "procgen/hallways.h"

int main() {
  Engine engine = Engine();
  HallwayGenerator hallwayGenerator;
  hallwayGenerator.generate(engine.getScene(), 580085);
  //hallwayGenerator.renderMap(engine.getScene().roomManager.active_rooms);
  engine.run();
  return EXIT_SUCCESS;
}