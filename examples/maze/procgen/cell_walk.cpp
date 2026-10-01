#include "cell_walk.h"
#include <cstdlib>
#include <algorithm>
#include "cell.h"

void CellWalk::fillCells(std::vector<uint32_t>& visited, float spawn_chance, int max_cells)
{
  std::vector<uint32_t> unvisited = {};
  unvisited.emplace_back(0);

  // generate cells
  while (!unvisited.empty() && max_cells > 0)
  {
    // basic depth first search algo
    uint32_t cell_hash = unvisited.back();
    const Cell cell(cell_hash);
    unvisited.pop_back();

    // determine room cells
    visited.emplace_back(cell_hash);
    max_cells--;

    for (int i = 0; i < 4; i++)
    {
      uint32_t neighbor_hash = cell.getRoomCellInDirection(i).hash();
      bool is_room_visited = std::ranges::contains(visited, neighbor_hash);
      if (is_room_visited)
        continue;

      // chance to create new room
      if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < spawn_chance)
        unvisited.push_back(neighbor_hash);
    }
  }
}