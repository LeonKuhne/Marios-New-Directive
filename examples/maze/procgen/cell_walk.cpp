#include "cell_walk.h"

void CellWalk::fillCells(std::vector<Cell>& visited, float spawn_chance, int max_cells)
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
      Cell room_cell = Cell::getRoomCellInDirection(cell, i);
      bool is_room_visited = std::ranges::contains(visited, room_cell);
      if (is_room_visited)
        continue;

      // chance to create new room
      if (rand() / static_cast<float>(RAND_MAX) < spawn_chance)
        unvisited.push_back(room_cell);
    }
  }
}