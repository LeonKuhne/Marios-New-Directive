#include "room.h"

void Room::addShape(ShapeData& shape) {
  shapes.emplace_back(shape);
}
std::vector<ShapeData>& Room::getShapes() {
  return shapes;
}