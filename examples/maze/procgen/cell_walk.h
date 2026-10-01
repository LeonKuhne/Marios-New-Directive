#pragma once

#include <cstdint>
#include <vector>

class CellWalk
{
public:
  static void fillCells(std::vector<uint32_t>& visited, float spawn_chance, int max_cells);
};