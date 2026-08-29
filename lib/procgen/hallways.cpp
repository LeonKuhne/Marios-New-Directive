#include "hallways.h"
#include "lib/lights/light.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "lib/engine/config.h"

static std::pair<int, int> getRoomCellInDirection(std::pair<int, int> cell, int i) {
  switch (i) {
    case 0: return std::make_pair(cell.first + 1, cell.second);
    case 1: return std::make_pair(cell.first, cell.second + 1);
    case 2: return std::make_pair(cell.first - 1, cell.second);
    case 3: return std::make_pair(cell.first, cell.second - 1);
    default: return cell;
  }
}

void HallwayGenerator::generate(Scene& scene)
{
  int max_rooms = 1000;
  const float spawn_room_chance = 0.5f;

  std::map<std::pair<int, int>, ShapeData> visited = {};
  std::vector<std::pair<int, int>> unvisited = {};
  unvisited.emplace_back(0, 0);

  while (!unvisited.empty() && max_rooms > 0)
  {
    // basic depth first search algo
    std::pair<int, int> cell = unvisited.back();
    unvisited.pop_back();

    // generate a room
    visited[cell] = generateRoom(scene, cell);
    max_rooms--;

    for (int i = 0; i < 4; i++)
    {
      std::pair<int, int> room_cell = getRoomCellInDirection(cell, i);
      bool is_room_visited = visited.find(room_cell) != visited.end();
      if (is_room_visited)
        continue;

      // chance to create new room
      if (static_cast<float>(rand()) / RAND_MAX < spawn_room_chance)
        unvisited.push_back(room_cell);
    }
  }

  for (auto& [cell, floor] : visited)
  {
    for (int i = 0; i < 4; i++)
    {
      // if there isnt a room in this direction, generate a wall
      std::pair<int, int> room_cell = getRoomCellInDirection(cell, i);
      bool is_room_visited = visited.find(room_cell) != visited.end();
      if (is_room_visited)
        continue;
      generateWall(scene, floor, i);
    }
  }

  scene.light_manager.updateLights();
}

ShapeData HallwayGenerator::generateRoom(Scene& scene, std::pair<int, int>& cell)
{
  const float room_size = 3.0f;
  glm::vec3 pos = glm::vec3(cell.first * room_size, 0.0f, cell.second * room_size);

  // create floor
  ShapeData floor = generateFloor(scene, pos, room_size, room_size);
  scene.shapes.add(floor);

  // create ceiling
  ShapeData ceiling = generateCeiling(scene, pos, room_size, room_size);
  scene.shapes.add(ceiling);

  // add light
  const float spawn_light_chance = 0.1f;
  if (static_cast<float>(rand()) / RAND_MAX < spawn_light_chance)
  {
    scene.light_manager.add(Light{.pos = glm::vec3(pos.x, pos.y + 1.0f, pos.z), .intensity = 5000.0f});
  }

  return floor;
}

void HallwayGenerator::generateWall(Scene& scene, ShapeData& base, int idx)
{
  Edge edge = getEdge(base, idx);
  ShapeData wall = Config::floor;
  const float height = Config::HallwaySettings::wall_height;
  if (idx % 2 == 0)
    wall.scale = glm::vec3(height, 0.0f, edge.size);
  else
    wall.scale = glm::vec3(edge.size, 0.0f, height);
  glm::vec3 world_offset = base.rotation * (edge.offset + glm::vec3(0, height / 2.0f, 0));
  wall.pos = base.pos + world_offset;
  wall.rotation = base.rotation * glm::rotation(
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::normalize(-edge.offset)
  );
  scene.data_points.finishPlane(wall);
  scene.shapes.add(wall);
}

Edge HallwayGenerator::getEdge(ShapeData& plane, int idx)
{
  glm::vec3 offset = glm::vec3(0.0f);
  float size;
  // along x
  if (idx % 2 == 0) {
    offset.x = plane.scale.x / 2.0f;
    size = plane.scale.z;
  // along z
  } else {
    offset.z = plane.scale.z / 2.0f;
    size = plane.scale.x;
  // negative direction
  } if (idx > 1) 
    offset = -offset;
  return Edge{idx, offset, size};
}

ShapeData HallwayGenerator::generateFloor(Scene& scene, glm::vec3 position, float width, float length)
{
  ShapeData floor = Config::floor;
  scene.data_points.finishPlane(floor);
  floor.pos = position;
  floor.scale.x = width;
  floor.scale.z = length;
  return floor;
}

ShapeData HallwayGenerator::generateCeiling(Scene& scene, glm::vec3 position, float width, float length)
{
  ShapeData ceiling = generateFloor(scene, position, width, length);
  ceiling.pos.y += Config::HallwaySettings::wall_height;
  ceiling.rotation = glm::rotate(ceiling.rotation, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  return ceiling;
}
