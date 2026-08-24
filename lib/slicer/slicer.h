#pragma once

#include <vector>
#include "lib/shapes/shape.h"
#include "lib/shapes/shape_data.h"
#include "lib/gpu/transfer_vec.h"
#include "util.h"

typedef struct
{
  glm::vec3 a;
  glm::vec3 b;
  glm::vec3 c;
} SliceInfo;

class Slicer
{
private:
  Plane plane;
  Shape *shape;
  DataPoints &data_points;

public:

  Slicer(DataPoints &data_points) : data_points(data_points) {};

  void select(Shape *shape) { this->shape = shape; }
  void align(const SliceInfo &info);
  bool slice(Shape *slices[2]);

private:
  bool slice_pass(SDL_GPUCopyPass *pass, ShapeData shape_data[2]);
};