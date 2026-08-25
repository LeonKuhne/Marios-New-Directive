#include "hallways.h"

#include <functional>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "lib/engine/config.h"

void HallwayGenerator::generate(Scene& scene, glm::vec3 position)
{
    const float width = 5.0f;
    const float length = 20.0f;

    // create floor
    ShapeData floor = generateFloor(scene, position, width, length);
    scene.shapes.add(floor);

    // create ceiling
    ShapeData ceiling = generateCeiling(scene, position, width, length);
    scene.shapes.add(ceiling);

    generateWalls(scene, floor, Config::HallwaySettings::wall_height);
}

void HallwayGenerator::generateWalls(Scene& scene, ShapeData& base, float height)
{
    glm::vec3 base_normal = base.rotation * glm::vec3(0.0f, 1.0f, 0.0f);

    // for each edge of plane
    eachEdge(base, [&base, &scene, height, &base_normal](Edge edge){
        ShapeData wall = Config::floor;
        if (edge.idx % 2 == 0)
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
    });
}

void HallwayGenerator::eachEdge(ShapeData& plane, std::function<void(Edge)> callback)
{
    for (int i = 0; i < 4; ++i) // x, z, -x, -z
    {
        glm::vec3 offset = glm::vec3(0.0f);
        float size;
        // along x
        if (i % 2 == 0) {
            offset.x = plane.scale.x / 2.0f;
            size = plane.scale.z;
        // along z
        } else {
            offset.z = plane.scale.z / 2.0f;
            size = plane.scale.x;
        // negative direction
        } if (i > 1) 
            offset = -offset;
        callback(Edge{i, offset, size});
    }
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