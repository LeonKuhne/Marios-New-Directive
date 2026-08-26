#pragma once

#include "lib/shapes/shape_data.h"
#include "lib/scene/scene.h"
#include <functional>


struct Edge
{
    int idx;
    glm::vec3 offset;
    float size;
};

class HallwayGenerator
{
public:
    static void generate(Scene& scene);
private:
    static ShapeData generateRoom(Scene& scene, std::pair<int, int>& cell);
    static void generateWall(Scene& scene, ShapeData& base, int idx);
    static Edge getEdge(ShapeData& plane, int idx);
    static ShapeData generateFloor(Scene& scene, glm::vec3 position, float width, float length);
    static ShapeData generateCeiling(Scene& scene, glm::vec3 position, float width, float length);
};