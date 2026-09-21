#include "hallways.h"
#include "lib/lights/light.h"
#include <SDL3/SDL_stdinc.h>
#include <stdexcept>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "lib/engine/config.h"
#include <algorithm>

static Cell getRoomCellInDirection(Cell cell, int i) {
  switch (i) {
    case 0: return std::make_pair(cell.first + 1, cell.second);
    case 1: return std::make_pair(cell.first, cell.second + 1);
    case 2: return std::make_pair(cell.first - 1, cell.second);
    case 3: return std::make_pair(cell.first, cell.second - 1);
    default: return cell;
  }
}

void HallwayGenerator::generate(Scene& scene, uint seed)
{
  srand(seed);

  int max_rooms = 1000;
  const float spawn_room_chance = 0.5f;

  std::vector<Cell> visited = {};
  fillCells(visited, spawn_room_chance, max_rooms);

  // construct rooms
  for (Cell& cell : visited)
  {
    // generate room shapes
    Room room;
    decorateRoom(room, cell, visited, scene.light_manager);

    // add room shapes to scene
    for (ShapeData& shape : room.getShapes())
    {
      scene.plane_builder.build(shape);
      scene.shapes.add(shape);
    }
  }

  // register lights
  if (scene.light_manager.lights.empty()) {
    throw std::runtime_error("Failed hallway generation: no lights");
  }
  scene.light_manager.updateLights();

  SDL_Log("Generated hallways with %zu lights, and %zu cells", scene.light_manager.lights.size(), visited.size());
}

void HallwayGenerator::fillCells(std::vector<Cell>& visited, float spawn_chance, int max_cells)
{
  std::vector<Cell> unvisited = {};
  unvisited.emplace_back(0, 0);

  // generate cells
  while (!unvisited.empty() && max_cells > 0)
  {
    // basic depth first search algo
    Cell cell = unvisited.back();
    unvisited.pop_back();

    // determine room cells
    visited.emplace_back(cell);
    max_cells--;

    for (int i = 0; i < 4; i++)
    {
      Cell room_cell = getRoomCellInDirection(cell, i);
      bool is_room_visited = std::ranges::contains(visited, room_cell);
      if (is_room_visited)
        continue;

      // chance to create new room
      if (static_cast<float>(rand()) / RAND_MAX < spawn_chance)
        unvisited.push_back(room_cell);
    }
  }
}

void HallwayGenerator::decorateRoom(Room& room, Cell& cell, std::vector<Cell>& visited, LightManager& light_manager)
{
  const float room_size = 3.0f;
  glm::vec3 pos = glm::vec3(static_cast<float>(cell.first) * room_size, 0.0f, static_cast<float>(cell.second) * room_size);

  // create floor
  ShapeData floor = createFloor(pos, room_size, room_size);
  room.addShape(floor);

  // create ceiling
  ShapeData ceiling = createCeiling(pos, room_size, room_size);
  room.addShape(ceiling);

  // add walls
  for (int i = 0; i < 4; i++)
  {
    // if there isnt a room in this direction, generate a wall
    Cell room_cell = getRoomCellInDirection(cell, i);
    if (std::ranges::contains(visited, room_cell))
      continue;
    ShapeData wall = createWall(floor, i);
    room.addShape(wall);
  }

  // add light
  const float spawn_light_chance = 0.1f;
  if (static_cast<float>(rand()) / RAND_MAX < spawn_light_chance)
  {
    light_manager.add(Light{.pos = glm::vec3(pos.x, pos.y + 1.0f, pos.z), .intensity = 5000.0f});
  }
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

ShapeData HallwayGenerator::createFloor(glm::vec3 position, float width, float length)
{
  ShapeData floor = Config::floor;
  floor.pos = position;
  floor.scale.x = width;
  floor.scale.z = length;
  return floor;
}

ShapeData HallwayGenerator::createCeiling(glm::vec3 position, float width, float length)
{
  ShapeData ceiling = createFloor(position, width, length);
  ceiling.pos.y += Config::HallwaySettings::wall_height;
  ceiling.rotation = glm::rotate(ceiling.rotation, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  return ceiling;
}

ShapeData HallwayGenerator::createWall(ShapeData& base, int idx)
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
  return wall;
}