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
    static void generateRoom(Scene& scene, std::pair<int, int> cell, int max_depth, std::vector<std::pair<int, int>> visited);
    static void generateWalls(Scene& scene, ShapeData& base, std::pair<int, int> cell, int max_depth, std::vector<std::pair<int, int>> visited);
    static void eachEdge(ShapeData& plane, std::function<void(Edge)> callback);
    static ShapeData generateFloor(Scene& scene, glm::vec3 position, float width, float length);
    static ShapeData generateCeiling(Scene& scene, glm::vec3 position, float width, float length);
};