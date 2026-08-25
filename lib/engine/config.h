#pragma once

#include <sys/types.h>
#include "lib/shapes/shape_data.h"

enum ShapeType : uint8_t
{
  GRAVITON = 1,
  ASTEROID = 2,
  PLAYER = 3,
  FLOOR = 4,
};

struct Config
{
  static constexpr glm::vec3 player_spawn_pos = glm::vec3(0.0f, 20.0f, 0.0f);
  static constexpr float player_walk_accel = 0.5f;
  static constexpr float player_sprint_accel = 5.0f;
  static constexpr float player_max_speed = 3.0f;
  static constexpr float player_jump_strength = 3.0f;

  static constexpr float ground_friction = 0.7f;
  static constexpr float player_friction = 1.0f;

  static constexpr ushort point_cloud_size = 10000;
  static constexpr float point_cloud_min_radius = 0.5f;


  static const ShapeData graviton;
  static const ShapeData asteroid;
  static const ShapeData floor;
};

inline const ShapeData Config::graviton = {
    .type = ShapeType::GRAVITON,
    .pos = glm::vec3(0.0f), 
    .scale = glm::vec3(10.0f),
    .density = 1000.0f,
    .color = {0.0f, 0.0f, 1.0f, 1.0f},
};

inline const ShapeData Config::asteroid = {
    .type = ShapeType::ASTEROID,
    .scale = glm::vec3(0.5f),
    .density = 1.0f,
    .color = {1.0f, 1.0f, 0.0f, 1.0f},
};

inline const ShapeData Config::floor = {
    .type = ShapeType::FLOOR,
    .pos = {0.0f, -2.0f, 0.0f},
    .rotation = glm::angleAxis(glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    .scale = {10.0f, 0.0f, 10.0f},
    .density = 1000.0f,
    .color = {0.0f, 0.0f, 1.0f, 1.0f},
};