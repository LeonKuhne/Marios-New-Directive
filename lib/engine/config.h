#pragma once

#include <sys/types.h>
#include "lib/shapes/shape_data.h"

enum ShapeType : uint8_t
{
  GRAVITON = 1,
  ASTEROID = 2,
  PLAYER = 3,
  PLANE = 4,
};

struct Config
{
  struct PlayerSettings
  {
    static constexpr glm::vec3 spawn_pos = glm::vec3(0.0f, 0.5f, 0.0f);
    static constexpr float walk_accel = 0.5f;
    static constexpr float sprint_accel = 2.0f;
    static constexpr float max_speed = 6.0f;
    static constexpr float air_movement_factor = 0.25f;
    static constexpr float height = 1.8f;
    static constexpr float radius = 0.3f;
    static constexpr float eye_height = 1.6f;
    static constexpr float jump_strength = 7.0f;
    static constexpr uint64_t jump_cooldown = 250; // in milliseconds
  };

  static constexpr float ground_friction = 0.7f;

  static constexpr ushort point_cloud_size = 10000;
  static constexpr float point_cloud_min_radius = 0.5f;

  struct HallwaySettings 
  {
    static constexpr float wall_height = 3.0f;
  };

  static inline const ShapeData graviton = {
      .type = ShapeType::GRAVITON,
      .pos = glm::vec3(0.0f), 
      .scale = glm::vec3(10.0f),
      .density = 1000.0f,
      .color = {0.0f, 0.0f, 1.0f, 1.0f},
  };

  static inline const ShapeData asteroid = {
      .type = ShapeType::ASTEROID,
      .scale = glm::vec3(0.5f),
      .density = 1.0f,
      .color = {1.0f, 1.0f, 0.0f, 1.0f},
  };

  static inline const ShapeData floor = {
      .type = ShapeType::PLANE,
      .pos = {0.0f, 0.0f, 0.0f},
      //.rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
      .scale = {10.0f, 0.0f, 10.0f},
      .density = 1000.0f,
      .color = {0.0f, 0.0f, 1.0f, 1.0f},
  };
};