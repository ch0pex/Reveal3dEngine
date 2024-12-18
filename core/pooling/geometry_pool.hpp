/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file transform_pool.hpp
 * @version 1.0
 * @date 29/10/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"
#include "render/mesh.hpp"

namespace reveal3d::core::geometry {

class Pool {
public:
  using init_info  = render::Mesh;
  using gpu_stored = std::true_type;

  render::Material& material(id_t const id) { return materials_.at(id::index(id)); }

  render::Mesh& mesh(id_t const id) { return meshes_.at(id::index(id)); }

  std::span<render::SubMesh> subMeshes(id_t const id) {
    return std::span {sub_meshes_.begin() + id::index(id), sub_meshes_.begin() + id::index(id) + 1};
  }

  [[nodiscard]] u64 vertices() const { return total_vertices_; }

  [[nodiscard]] u64 triangles() const { return total_triangles_; }

protected:
  [[nodiscard]] u32 countData() const { return meshes_.size(); }

  void addData(init_info const& init_info) {
    materials_.emplace_back();
    sub_meshes_.emplace_back(render::SubMesh {
      .vertex_pos  = 0,
      .index_pos   = 0,
      .index_count = static_cast<u32>(init_info.indices.size()),
      .visible     = true,
      .shader      = 1 // Opaque
    });
    total_vertices_ += init_info.vertex_count;
    total_triangles_ += init_info.triangle_count;
    meshes_.push_back(std::move(init_info));
  }

  void removeData(u32 const id) {
    index_t const idx = id::index(id);
    total_vertices_ -= meshes_.at(idx).vertex_count;
    total_triangles_ -= meshes_.at(idx).triangle_count;
    materials_.unordered_remove(idx);
    meshes_.unordered_remove(idx);
    sub_meshes_.unordered_remove(idx);
  }

private:
  utl::vector<render::Material> materials_;
  utl::vector<render::SubMesh> sub_meshes_;
  utl::vector<render::Mesh> meshes_;
  u64 total_vertices_ {0};
  u64 total_triangles_ {0};
};

} // namespace reveal3d::core::geometry
