#pragma once

#include "lib/shapes/shape_data.h"
#include "portal.h"

class Room {
private:
  std::vector<ShapeData> shapes;
  std::vector<Portal> portals;
public:
  void addShape(ShapeData& shape);
  std::vector<ShapeData>& getShapes();
};