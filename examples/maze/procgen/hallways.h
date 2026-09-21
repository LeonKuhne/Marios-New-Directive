#pragma once

#include "lib/lights/light_manager.h"
#include "lib/rooms/room.h"
#include "lib/shapes/shape_data.h"
#include "lib/scene/scene.h"

struct Edge
{
    int idx;
    glm::vec3 offset;
    float size;
};

using Cell = std::pair<int, int>;

class HallwayGenerator
{
public:
    static void generate(Scene& scene, uint seed);
private:
    static void fillCells(std::vector<Cell>& visited, float spawn_chance, int max_cells);
    static Edge getEdge(ShapeData& plane, int idx);

    static void decorateRoom(Room& room, Cell& cell, std::vector<Cell>& visited, LightManager& light_manager);

    static ShapeData createFloor(glm::vec3 position, float width, float length);
    static ShapeData createCeiling(glm::vec3 position, float width, float length);
    static ShapeData createWall(ShapeData& base, int idx);
};