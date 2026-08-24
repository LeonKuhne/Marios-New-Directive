#include "poly.h"

struct SampleInfo
{
  std::vector<ushort> *indices;
  ushort max_points;
  ushort random_points_start;
  ushort random_points_size;
  const std::vector<glm::vec3> *all_vertices;
};

// asumes that many vertices exist
static void sampleCloud(SampleInfo info)
{
  // select random indices to form point cloud
  std::unordered_set<ushort> point_cloud;
  while (point_cloud.size() < info.max_points)
  { 
    ushort index = info.random_points_start + rand() % (info.random_points_size);
    glm::vec3 vertex = (*info.all_vertices)[index];
    point_cloud.insert(index);
  }

  // convex hull point cloud as poly
  convexHull({info.indices, point_cloud, info.all_vertices});
}

ShapeData polyCreate(PolyInfo info)
{
  DataPoints &pts = info.data_points;

  std::vector<ushort> poly_indices;
  sampleCloud({&poly_indices, info.max_vertices, 0, Config::point_cloud_size, &pts.all_vertices});
  SDL_GPUBuffer* poly_index_buffer;
  pts.load([&pts, poly_indices, &poly_index_buffer](SDL_GPUCopyPass *pass) {
    poly_index_buffer = vecToGPU<ushort>({pts.gpu, pass, SDL_GPU_BUFFERUSAGE_INDEX, &poly_indices});
    return false;
  });
  return pts.finishShape(info.base, poly_indices, poly_index_buffer);
}