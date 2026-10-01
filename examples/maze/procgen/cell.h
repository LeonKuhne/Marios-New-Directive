#pragma once

#include <cstdint>

class Cell
{
public:
  int16_t x;
  int16_t y;


  Cell(uint32_t hash) {
    x = static_cast<int16_t>(hash >> 16);
    y = static_cast<int16_t>(hash & 0xFFFF);
  }

  Cell(int16_t x, int16_t y) : x(x), y(y) {}

  Cell getRoomCellInDirection(int i) const {
    switch (i) {
      case 0: return {static_cast<int16_t>(x + 1), y};
      case 1: return {static_cast<int16_t>(x), static_cast<int16_t>(y + 1)};
      case 2: return {static_cast<int16_t>(x - 1), y};
      case 3: return {x, static_cast<int16_t>(y - 1)};
      default: return {x, y};
    }
  }

  uint32_t hash() const {
    uint32_t value = (uint16_t(x) << 16) | uint16_t(y);
    return static_cast<uint32_t>(value);
  }
};