#include "hallways.h"

#include <functional>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "lib/engine/config.h"

void HallwayGenerator::generate(Scene& scene)
{
    int max_depth = 16;
    std::vector<std::pair<int, int>> visited = {};
    std::pair<int, int> cell = std::make_pair(0, 0);
    visited.push_back(cell);
    generateRoom(scene, cell, max_depth, visited);
}

/*


    NOTE

    i cant just mark cells as visited i need to mark edges
    otherwise you cant tell if a cell that is visited has an open or closed path, so placing walls doesnt make sense


*/

void HallwayGenerator::generateRoom(Scene& scene, std::pair<int, int> cell, int max_depth, std::vector<std::pair<int, int>> visited)
{
    if (max_depth <= 0)
        return;

    const float width = 5.0f;
    const float length = 5.0f;
    glm::vec3 pos = glm::vec3(cell.first * width, 0.0f, cell.second * length);

    // create floor
    ShapeData floor = generateFloor(scene, pos, width, length);
    scene.shapes.add(floor);

    // create ceiling
    ShapeData ceiling = generateCeiling(scene, pos, width, length);
    scene.shapes.add(ceiling);

    generateWalls(scene, floor, cell, max_depth, visited);
}

void HallwayGenerator::generateWalls(Scene& scene, ShapeData& base, std::pair<int, int> cell, int max_depth, std::vector<std::pair<int, int>> visited)
{
    glm::vec3 base_normal = base.rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    std::vector<std::pair<int, int>> next_cells = {};

    // for each edge of plane
    eachEdge(base, [&base, &scene, cell, max_depth, &visited, &next_cells](Edge edge){
        std::pair<int, int> cell_delta = std::make_pair(0, 0);
        if (edge.idx % 2 == 0) cell_delta.first = 1;
        else cell_delta.second = 1;
        if (edge.idx > 1) {
            cell_delta.first = -cell_delta.first;
            cell_delta.second = -cell_delta.second;
        }
        std::pair<int, int> next_cell = std::make_pair(cell.first + cell_delta.first, cell.second + cell_delta.second);

        bool is_visited = std::find(visited.begin(), visited.end(), next_cell) != visited.end();
        if (is_visited)
            return;

        // chance to generate room instead of wall
        if (rand() % 2 == 0)
        {
            visited.push_back(next_cell);
            next_cells.push_back(next_cell);
            return;
        }

        // fill wall
        ShapeData wall = Config::floor;
        const float height = Config::HallwaySettings::wall_height;
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

    for (auto& next_cell : next_cells)
        HallwayGenerator::generateRoom(scene, next_cell, max_depth - 1, visited);
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