#pragma once

class Cell
{
public:
  int x;
  int y;

  Cell getRoomCellInDirection(int i) {
    switch (i) {
      case 0: return Cell{x + 1, y};
      case 1: return Cell{x, y + 1};
      case 2: return Cell{x - 1, y};
      case 3: return Cell{x, y - 1};
      default: return Cell{x, y};
    }
  }
};