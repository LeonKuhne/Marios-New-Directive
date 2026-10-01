#include "render_map.h"
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

struct Bounds
{
  int16_t min_x;
  int16_t max_x;
  int16_t min_y;
  int16_t max_y;
};

Bounds getBounds(const std::map<uint32_t, Room*>& rooms)
{
  const Cell cell(rooms.begin()->first);
  Bounds bounds{
    .min_x=cell.x,
    .max_x=cell.x,
    .min_y=cell.y,
    .max_y=cell.y,
  };

  for (const auto& [cell_hash, room] : rooms)
  {
    const Cell cell(cell_hash);
    bounds.min_x = std::min(bounds.min_x, cell.x);
    bounds.max_x = std::max(bounds.max_x, cell.x);
    bounds.min_y = std::min(bounds.min_y, cell.y);
    bounds.max_y = std::max(bounds.max_y, cell.y);
  }
  return bounds;
}

std::set<const Room*> getDirectNeighbors(const std::vector<Room*>& active_rooms)
{
  std::set<const Room*> direct_neighbors;
  for (const Room* active_room : active_rooms)
    for (const Portal* portal : active_room->portals)
      direct_neighbors.emplace(&portal->otherRoom(*active_room));
  return direct_neighbors;
}

bool isActive(const std::vector<Room*>& active_rooms, const Room& room)
{
  return std::ranges::contains(active_rooms, &room);
}

size_t activeIndex(const std::vector<Room*>& active_rooms, const Room& room)
{
  auto active_room = std::ranges::find(active_rooms, &room);
  return active_room == active_rooms.end()
    ? active_rooms.size()
    : static_cast<size_t>(active_room - active_rooms.begin());
}

bool isVisible(const std::vector<Room*>& active_rooms, const Room& room)
{
  return isActive(active_rooms, room) || std::ranges::any_of(active_rooms,
    [&](const Room* active_room) { return room.isVisibleFrom(*active_room); });
}

std::string RenderMap::cellSymbol(
  const Rooms& rooms,
  const std::vector<Room*>& active_rooms,
  const Cell& cell,
  const std::set<const Room*>& direct_neighbors, size_t& visible_count,
  size_t& missing_direct_count
) {
  auto room = rooms.find(cell.hash());
  if (room == rooms.end())
    return "  ";

  if (isActive(active_rooms, *room->second))
    return "██";

  bool direct_neighbor = direct_neighbors.contains(room->second);
    if (direct_neighbor && !isVisible(active_rooms, *room->second))
  {
    missing_direct_count++;
    return "▓▓";
  }

  if (isVisible(active_rooms, *room->second))
  {
    visible_count++;
    return "▒▒";
  }

  return "░░";
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

void RenderMap::renderAscii(
  const Rooms& rooms, 
  const std::vector<Room*>& active_rooms
) {
  if (rooms.empty())
    return;

  Bounds bounds = getBounds(rooms);
  std::set<const Room*> direct_neighbors = getDirectNeighbors(active_rooms);
  size_t visible_count = 0;
  size_t missing_direct_count = 0;
  SDL_Log("Visibility map for %zu active rooms:", active_rooms.size());
  for (const auto& [cell_hash, room] : rooms)
    if (isActive(active_rooms, *room))
      SDL_Log("Active cell: (%d, %d)", cell_hash >> 16, cell_hash & 0xFFFF);
  for (int16_t y = bounds.max_y; y >= bounds.min_y; --y)
  {
    std::string row;
    for (int16_t x = bounds.min_x; x <= bounds.max_x; ++x)
      row += cellSymbol(rooms, active_rooms, {x, y}, direct_neighbors, visible_count, missing_direct_count);
    SDL_Log("%s", row.c_str());
  }
  SDL_Log("Visible cells: %zu / %zu", visible_count, rooms.size());
  SDL_Log("Direct neighbors missing from visibility: %zu", missing_direct_count);
}

bool RenderMap::renderPng(
  const Rooms& rooms,
  const std::vector<Room*>& active_rooms,
  const char* path
) {
  if (rooms.empty())
    return false;

  constexpr int cell_size = 16;
  constexpr int cell_gap = 1;
  Bounds bounds = getBounds(rooms);
  int columns = bounds.max_x - bounds.min_x + 1;
  int rows = bounds.max_y - bounds.min_y + 1;
  int width = columns * cell_size;
  int height = rows * cell_size;
  std::set<const Room*> direct_neighbors = getDirectNeighbors(active_rooms);
  std::vector<std::uint8_t> pixels(static_cast<size_t>(width) * height * 4, 0);
  std::array<std::uint8_t, 4> color_active{255, 210, 55, 255};
  std::array<std::uint8_t, 4> color_active_secondary{255, 145, 45, 255};
  std::array<std::uint8_t, 4> color_inactive{55, 60, 70, 255};
  std::array<std::uint8_t, 4> color_visible{65, 155, 245, 255};
  std::array<std::uint8_t, 4> color_direct_neighbor{225, 75, 75, 255};

  for (int16_t y = bounds.min_y; y <= bounds.max_y; ++y)
  {
    for (int16_t x = bounds.min_x; x <= bounds.max_x; ++x)
    {
      const Cell cell(x, y);
      auto it = rooms.find(cell.hash());
      if (it == rooms.end())
        continue;
      const Room& room = *it->second;

      size_t active_index = activeIndex(active_rooms, room);
      std::array<std::uint8_t, 4> color;
      if (active_index == 0)
        color = color_active;
      else if (active_index < active_rooms.size())
        color = color_active_secondary;
      else if (isVisible(active_rooms, room))
        color = color_visible;
      else if (direct_neighbors.contains(&room))
        color = color_direct_neighbor;
      else
        color = color_inactive;

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
