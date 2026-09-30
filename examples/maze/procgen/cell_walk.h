#pragma once

#include "cell.h"
#include <vector>

class CellWalk
{
public:
  static void fillCells(std::vector<Cell>& visited, float spawn_chance, int max_cells);
};