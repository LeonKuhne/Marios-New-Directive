#include "hallways.h"

#include <functional>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "lib/engine/config.h"

void generateHallways(Scene& scene, ShapeData& plane)
{
    generateWalls(scene, plane, Config::HallwayGenerator::wall_height);
}

void generateWalls(Scene& scene, ShapeData& base, float height)
{
    glm::vec3 base_normal = base.rotation * glm::vec3(0.0f, 1.0f, 0.0f);

    // for each edge of plane
    eachEdge(base, [&base, &scene, height, &base_normal](Edge edge){
        ShapeData wall = Config::floor;
        if (edge.idx % 2 == 0)
            wall.scale = glm::vec3(height, 0.0f, edge.size);
        else
            wall.scale = glm::vec3(edge.size, 0.0f, height);
        wall.pos = base.pos + edge.offset + glm::vec3(0.0f, height / 2.0f, 0.0f);
        wall.rotation = glm::rotation(base_normal, glm::normalize(-edge.offset));
        scene.shapes.add(new Shape(scene.data_points.finishPlane(wall)));
    });
}

void eachEdge(ShapeData& plane, std::function<void(Edge)> callback)
{
    for (int i = 0; i < 4; ++i) // x, z, -x, -z
    {
        glm::vec3 offset = glm::vec3(0.0f);
        float size;

        // along x
        if (i % 2 == 0) 
        {
            offset.x = plane.scale.x / 2.0f;
            size = plane.scale.x;
        }

        // along z
        else 
        {
            offset.z = plane.scale.z / 2.0f;
            size = plane.scale.z;
        }

        // negative direction
        if (i > 1) 
            offset = -offset;

        callback(Edge{i, offset, size});
    }
}