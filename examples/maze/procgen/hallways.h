#pragma once

#include "map.h"
#include "lib/shapes/shape_data.h"
#include "lib/scene/scene.h"
#include "cell.h"

struct Edge
{
    int idx;
    glm::vec3 offset;
    float size;
};

class HallwayGenerator
{
public:
    void generate(Scene& scene, uint seed);
    void renderMap(std::vector<Room*>& active_rooms);

private:
    std::map<const Cell, Room*> rooms;

    static Edge getEdge(const ShapeData& plane, int idx);

    static void decorateRoom(Room& room, const Cell& cell, std::map<const Cell, Room*>& rooms, Scene& scene);

    static ShapeData createFloor(glm::vec3 position, float width, float length);
    static ShapeData createCeiling(glm::vec3 position, float width, float length);
    static ShapeData createWall(const ShapeData& base, const ShapeData& floor, int idx);
};