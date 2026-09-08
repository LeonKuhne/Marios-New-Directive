#pragma once
#include "lib/shapes/shape.h"
#include <cstdint>
#include <vector>

class ECSManager
{
public:
  std::vector<uint64_t> entities;
  std::vector<btRigidBody*> bodies;
  std::vector<Shape*> shapes;
  ECSManager() = default;
};