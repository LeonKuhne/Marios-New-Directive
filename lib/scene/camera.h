#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "lib/render/window.h"
#include "lib/controls/mouse.h"
#include "player.h"

class Camera
{
  glm::mat4 projection;
  float fov_radians;
  Window &window;
  Mouse &mouse;
  Player &player;
  int mouse_look_callback_id = -1;

public:
  glm::vec3 pos;
  glm::mat4 view_projection;

  Camera(Window &window, Player &player, Mouse &mouse, float fov = 75.0f)
      : fov_radians(glm::radians(fov)),
        window(window),
        mouse(mouse),
        player(player),
        pos(player.getPosition())
  {
    window.addListener([this](uint w, uint h) { setAspect((float)w / (float)h); });
    enable();
  }

  void enable() {
    mouse_look_callback_id = mouse.onMove.add([this]() {
      float sensitivity = 0.1f;
      rotate(mouse.delta.x * sensitivity, mouse.delta.y * sensitivity);
    });
    SDL_SetWindowRelativeMouseMode(window.ctx.window, true);
  }

  void disable() {
    mouse.onMove.remove(mouse_look_callback_id);
    mouse_look_callback_id = -1;
    SDL_SetWindowRelativeMouseMode(window.ctx.window, false);
  }

  glm::vec3 screenToWorld(int x, int y, float depth = 0.98f)
  {
    // convert screen coordinates to NDC
    float ndc_x = (2.0f * x) / window.width - 1.0f;
    float ndc_y = 1.0f - (2.0f * y) / window.height;
    float ndc_z = depth;
    glm::vec4 clip_coords = glm::vec4(ndc_x, ndc_y, ndc_z, 1.0f);

    // convert to world coordinates
    glm::mat4 inv_view_proj = glm::inverse(view_projection);
    glm::vec4 world_coords = inv_view_proj * clip_coords;
    world_coords /= world_coords.w; // perspective divide

    return glm::vec3(world_coords);
  }

  void rotate(float delta_yaw, float delta_pitch)
  {
    glm::vec3 gravity = player.getGravity();
    glm::vec3 current_up = glm::dot(gravity, gravity) > 1e-6f
        ? glm::normalize(-gravity)
        : glm::vec3(0.0f, 1.0f, 0.0f);

    glm::quat yaw_rotation = glm::angleAxis(glm::radians(-delta_yaw), current_up);
    glm::vec3 yawed_forward = glm::normalize(glm::mat3_cast(yaw_rotation) * player.forward);

    glm::vec3 right = glm::cross(yawed_forward, current_up);
    if (glm::dot(right, right) <= 1e-6f)
      return;
    right = glm::normalize(right);

    float current_pitch = glm::degrees(glm::asin(glm::clamp(glm::dot(yawed_forward, current_up), -1.0f, 1.0f)));
    float target_pitch = glm::clamp(current_pitch - delta_pitch, -89.0f, 89.0f);
    float pitch_delta = target_pitch - current_pitch;

    glm::quat pitch_rotation = glm::angleAxis(glm::radians(pitch_delta), right);
    player.forward = glm::normalize(glm::mat3_cast(pitch_rotation) * yawed_forward);
  }

  void tick()
  {
    pos = player.getPosition();
    glm::vec3 previous_up = player.up;
    glm::vec3 gravity = player.getGravity();
    if (glm::dot(gravity, gravity) > 1e-6f)
      player.up = glm::normalize(-gravity);
    else
      player.up = glm::vec3(0.0f, 1.0f, 0.0f);

    if (glm::dot(previous_up, previous_up) <= 1e-6f)
      previous_up = player.up;
    else
      previous_up = glm::normalize(previous_up);

    float up_dot = glm::clamp(glm::dot(previous_up, player.up), -1.0f, 1.0f);
    if ((1.0f - up_dot) > 1e-7f)
    {
      glm::vec3 rotation_axis = glm::cross(previous_up, player.up);
      float axis_len2 = glm::dot(rotation_axis, rotation_axis);

      if (axis_len2 <= 1e-6f)
      {
        rotation_axis = glm::cross(previous_up, glm::vec3(1.0f, 0.0f, 0.0f));
        if (glm::dot(rotation_axis, rotation_axis) <= 1e-6f)
          rotation_axis = glm::cross(previous_up, glm::vec3(0.0f, 0.0f, 1.0f));
      }

      if (glm::dot(rotation_axis, rotation_axis) > 1e-6f)
      {
        glm::quat up_rotation = glm::angleAxis(glm::acos(up_dot), glm::normalize(rotation_axis));
        player.forward = glm::normalize(glm::mat3_cast(up_rotation) * player.forward);
      }
    }

    if (glm::dot(player.forward, player.forward) <= 1e-6f)
      player.forward = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), player.up));

    glm::vec3 camera_pos(pos.x, pos.y + player.eye_height, pos.z);
    glm::mat4 view = glm::lookAt(camera_pos, camera_pos + player.forward, player.up);
    view_projection = projection * view;
  }

private:
  void setAspect(float aspect)
  {
    projection = glm::perspective(fov_radians, aspect, 0.1f, 100.0f);
  }
};