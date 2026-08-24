#pragma once

#include <vector>
#include <unordered_set>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "lib/gpu/transfer_vec.h"
#include "cloud.h"
#include "convex_hull.h"
#include "lib/render/data_points.h"
#include "lib/gpu/transfer_vec.h"
#include "lib/shapes/shape.h"

struct PolyInfo
{
  ShapeData base;
  ushort max_vertices;
  DataPoints &data_points;
};

ShapeData polyCreate(PolyInfo info);