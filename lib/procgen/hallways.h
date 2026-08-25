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

void generateHallways(Scene& scene, ShapeData& plane);
void generateWalls(Scene& scene, ShapeData& plane, float height);
void eachEdge(ShapeData& plane, std::function<void(Edge)> callback);