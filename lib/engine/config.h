#pragma once

#include <sys/types.h>
#include "lib/shapes/shape_data.h"

struct Config
{
  enum Colliders : uint8_t
  {
    PLAYER = 0,
    PORTAL = 1,
    SURFACE = 2,
  };

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

  static inline const ShapeData floor = {
      .collider_type = Config::Colliders::SURFACE,
      .pos = {0.0f, 0.0f, 0.0f},
      //.rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
      .scale = {10.0f, 0.0f, 10.0f},
      .density = 1000.0f,
      .color = SDL_FColor(0.0f, 0.0f, 0.0f, 1.0f)
  };

  static inline const ShapeData portal = {
      .collider_type = Config::Colliders::PORTAL,
      .is_visible = false,
      .pos = {0.0f, 0.0f, 0.0f},
      //.rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
      .scale = {10.0f, 0.0f, 10.0f},
      .density = 0.0f,
      .color = SDL_FColor(1.0f, 0.0f, 0.0f, 1.0f)
  };
};