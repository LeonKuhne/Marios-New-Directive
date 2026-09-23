#include "lib/engine/engine.h"
#include "procgen/hallways.h"

int main() {
  Engine engine = Engine();
  HallwayGenerator::generate(engine.getScene(), 580085);
  engine.run();
  return EXIT_SUCCESS;
}