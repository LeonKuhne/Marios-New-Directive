#include "map.h"
#include <SDL3/SDL_log.h>
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <zlib.h>

namespace
{
struct Bounds
{
  int min_x;
  int max_x;
  int min_y;
  int max_y;
};

Bounds getBounds(const std::map<const Cell, Room&>& rooms)
{
  Bounds bounds{
    rooms.begin()->first.first,
    rooms.begin()->first.first,
    rooms.begin()->first.second,
    rooms.begin()->first.second,
  };

  for (const auto& [cell, room] : rooms)
  {
    bounds.min_x = std::min(bounds.min_x, cell.first);
    bounds.max_x = std::max(bounds.max_x, cell.first);
    bounds.min_y = std::min(bounds.min_y, cell.second);
    bounds.max_y = std::max(bounds.max_y, cell.second);
  }
  return bounds;
}

std::set<const Room*> getDirectNeighbors(const Room& active_room)
{
  std::set<const Room*> direct_neighbors;
  for (const Portal& portal : active_room.portals)
    direct_neighbors.emplace(&portal.destination);
  return direct_neighbors;
}

char cellSymbol(const std::map<const Cell, Room&>& rooms, Room& active_room, const Cell& cell,
                const std::set<const Room*>& direct_neighbors, size_t& visible_count,
                size_t& missing_direct_count)
{
  auto room = rooms.find(cell);
  if (room == rooms.end())
    return ' ';

  if (&room->second == &active_room)
    return 'A';

  bool direct_neighbor = direct_neighbors.contains(&room->second);
  if (direct_neighbor && !room->second.isVisibleFrom(active_room))
  {
    missing_direct_count++;
    return 'N';
  }

  if (room->second.isVisibleFrom(active_room))
  {
    visible_count++;
    return 'V';
  }

  return '.';
}

void writeBigEndian(std::vector<std::uint8_t>& output, std::uint32_t value)
{
  output.push_back(static_cast<std::uint8_t>(value >> 24));
  output.push_back(static_cast<std::uint8_t>(value >> 16));
  output.push_back(static_cast<std::uint8_t>(value >> 8));
  output.push_back(static_cast<std::uint8_t>(value));
}

void writeChunk(std::ofstream& file, const char type[4], const std::vector<std::uint8_t>& data)
{
  std::vector<std::uint8_t> chunk;
  chunk.reserve(4 + data.size());
  chunk.insert(chunk.end(), type, type + 4);
  chunk.insert(chunk.end(), data.begin(), data.end());

  std::uint32_t checksum = crc32(0L, Z_NULL, 0);
  checksum = crc32(checksum, chunk.data(), static_cast<uInt>(chunk.size()));

  std::uint32_t length = static_cast<std::uint32_t>(data.size());
  file.put(static_cast<char>(length >> 24));
  file.put(static_cast<char>(length >> 16));
  file.put(static_cast<char>(length >> 8));
  file.put(static_cast<char>(length));
  file.write(type, 4);
  if (!data.empty())
    file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
  file.put(static_cast<char>(checksum >> 24));
  file.put(static_cast<char>(checksum >> 16));
  file.put(static_cast<char>(checksum >> 8));
  file.put(static_cast<char>(checksum));
}

void setPixel(std::vector<std::uint8_t>& pixels, int width, int x, int y,
              std::array<std::uint8_t, 4> color)
{
  size_t offset = (static_cast<size_t>(y) * width + x) * 4;
  std::copy(color.begin(), color.end(), pixels.begin() + static_cast<std::ptrdiff_t>(offset));
}
}

void VisibilityMap::renderAscii(const std::map<const Cell, Room&>& rooms, Room& active_room)
{
  if (rooms.empty())
    return;

  Bounds bounds = getBounds(rooms);
  std::set<const Room*> direct_neighbors = getDirectNeighbors(active_room);
  size_t visible_count = 0;
  size_t missing_direct_count = 0;
  SDL_Log("Visibility map for active cell (0, 0):");
  for (int y = bounds.max_y; y >= bounds.min_y; --y)
  {
    std::string row;
    for (int x = bounds.min_x; x <= bounds.max_x; ++x)
      row += cellSymbol(rooms, active_room, {x, y}, direct_neighbors,
                        visible_count, missing_direct_count);
    SDL_Log("%s", row.c_str());
  }
  SDL_Log("Visible cells: %zu / %zu", visible_count, rooms.size());
  SDL_Log("Direct neighbors missing from visibility: %zu", missing_direct_count);
}

bool VisibilityMap::renderPng(const std::map<const Cell, Room&>& rooms, Room& active_room,
                              const char* path)
{
  if (rooms.empty())
    return false;

  constexpr int cell_size = 16;
  constexpr int cell_gap = 1;
  Bounds bounds = getBounds(rooms);
  int columns = bounds.max_x - bounds.min_x + 1;
  int rows = bounds.max_y - bounds.min_y + 1;
  int width = columns * cell_size;
  int height = rows * cell_size;
  std::set<const Room*> direct_neighbors = getDirectNeighbors(active_room);
  std::vector<std::uint8_t> pixels(static_cast<size_t>(width) * height * 4, 0);

  for (int y = bounds.min_y; y <= bounds.max_y; ++y)
  {
    for (int x = bounds.min_x; x <= bounds.max_x; ++x)
    {
      auto room = rooms.find({x, y});
      if (room == rooms.end())
        continue;

      std::array<std::uint8_t, 4> color;
      if (&room->second == &active_room)
        color = {255, 210, 55, 255};
      else if (room->second.isVisibleFrom(active_room))
        color = {65, 155, 245, 255};
      else if (direct_neighbors.contains(&room->second))
        color = {225, 75, 75, 255};
      else
        color = {55, 60, 70, 255};

      int pixel_x = (x - bounds.min_x) * cell_size;
      int pixel_y = (bounds.max_y - y) * cell_size;
      for (int row = 0; row < cell_size - cell_gap; ++row)
        for (int column = 0; column < cell_size - cell_gap; ++column)
          setPixel(pixels, width, pixel_x + column, pixel_y + row, color);
    }
  }

  std::vector<std::uint8_t> scanlines;
  scanlines.reserve(static_cast<size_t>(height) * (width * 4 + 1));
  for (int y = 0; y < height; ++y)
  {
    scanlines.push_back(0);
    auto row_start = static_cast<std::ptrdiff_t>(static_cast<size_t>(y) * width * 4);
    auto row_end = static_cast<std::ptrdiff_t>(static_cast<size_t>(y + 1) * width * 4);
    scanlines.insert(scanlines.end(), pixels.begin() + row_start, pixels.begin() + row_end);
  }

  uLong compressed_size = compressBound(scanlines.size());
  std::vector<std::uint8_t> compressed(compressed_size);
  if (compress2(compressed.data(), &compressed_size, scanlines.data(), scanlines.size(), Z_BEST_SPEED) != Z_OK)
    return false;
  compressed.resize(compressed_size);

  std::ofstream file(path, std::ios::binary);
  if (!file)
    return false;

  static constexpr std::array<std::uint8_t, 8> signature = {137, 80, 78, 71, 13, 10, 26, 10};
  file.write(reinterpret_cast<const char*>(signature.data()), signature.size());

  std::vector<std::uint8_t> header;
  writeBigEndian(header, width);
  writeBigEndian(header, height);
  header.insert(header.end(), {8, 6, 0, 0, 0});
  writeChunk(file, "IHDR", header);
  writeChunk(file, "IDAT", compressed);
  writeChunk(file, "IEND", {});
  return file.good();
}
