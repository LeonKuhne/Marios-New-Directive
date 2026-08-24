#pragma once

#include <sys/types.h>
#include "lib/shapes/shape_data.h"

//#define DRAW_GIZMOS 1

enum ShapeType
{
  GRAVITON = 1,
  ASTEROID = 2,
  PLAYER = 3,
};

struct Config
{
  static constexpr glm::vec3 player_spawn_pos = glm::vec3(0.0f, 20.0f, 0.0f);
  static constexpr float player_speed = 5.0f;
  static constexpr float player_sprint_speed = 12.0f;
  static constexpr float player_jump_strength = 3.0f;
  static constexpr float player_air_friction = 0.0f;
  static constexpr float player_ground_gravity_factor = 0.01f;
  static constexpr float player_ground_friction = 3.0f;
  static constexpr float player_ground_rolling_friction = 4.0f;

  static constexpr ushort point_cloud_size = 10000;
  static constexpr float point_cloud_min_radius = 0.5f;
  static constexpr ushort num_asteroids = 500;
  static constexpr ulong asteroid_tick_spawn_interval = 3000;

  static constexpr float asteroid_spawn_distance = 50.0f;
  static constexpr float asteroid_spawn_velocity = 50.0f;

  static const ShapeData graviton;
  static const ShapeData asteroid;
  static const ShapeData floor;
  static const ShapeData gizmo;
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
    .type = ShapeType::GRAVITON,
    .pos = {0.0f, -2.0f, 0.0f},
    .scale = {30.0f, 1.0f, 30.0f},
    .density = 1000.0f,
    .color = {0.0f, 0.0f, 1.0f, 1.0f},
};

inline const ShapeData Config::gizmo = {
    .scale = glm::vec3(0.1f),
    .color = {0.0f, 1.0f, 0.0f, 1.0f},
};
