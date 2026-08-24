#pragma once

#include <sys/types.h>
#include <vector>

class MeshBuilder {
  std::vector<std::vector<ushort>> faces;
  std::vector<ushort> indices;

public:
  void addFace(std::vector<ushort> face) { faces.push_back(face); } 
  void build(std::vector<ushort> *indices, ushort offset = 0);
};