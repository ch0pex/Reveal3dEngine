/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file geometry.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Geometry entity
 */

#pragma once

#include "core/scene.hpp"
#include "render/mesh.hpp"

#include <queue>
#include <set>
#include <span>
#include <vector>

namespace reveal3d::core {

class Geometry {
public:
  using init_info = render::Mesh;
  using pool_type = geometry::Pool;

  constexpr Geometry() : id_ {id::invalid} { }

  constexpr Geometry(id_t const id) : id_ {id} { }

  constexpr Geometry(Geometry const& other) : id_ {other.id()} { }

  constexpr Geometry(Geometry&& other) noexcept : id_ {other.id()} { }

  [[nodiscard]] constexpr bool isAlive() const { return id_ != id::invalid; }

  [[nodiscard]] constexpr id_t id() const { return id_; }

  [[nodiscard]] constexpr u32 entityIdx() const { return pool.getMappedId(id_); }

  Geometry& operator=(Geometry const& other) = default;

  Geometry& operator=(Geometry&& other) noexcept {
    id_ = other.id_;
    return *this;
  }

  // Note: for now we only will have one mesh and submesh, so we move the vector instead of appending it
  void addMesh(render::Mesh& mesh) const {
    render::SubMesh sub_mesh;
    pool.mesh(id_) = std::move(mesh);

    sub_mesh.vertex_pos  = vertexCount();
    sub_mesh.index_pos   = 0;
    sub_mesh.index_count = indexCount();
  }

  [[nodiscard]] u32 vertexCount() const { return pool.mesh(id_).vertices.size(); }

  [[nodiscard]] u32 indexCount() const { return pool.mesh(id_).indices.size(); }

  [[nodiscard]] std::span<render::SubMesh> subMeshes() const { return pool.subMeshes(id_); }

  [[nodiscard]] std::vector<render::Vertex>& vertices() const { return pool.mesh(id_).vertices; }

  [[nodiscard]] std::vector<u32>& indices() const { return pool.mesh(id_).indices; }

  [[nodiscard]] bool isVisible() const { return pool.subMeshes(id_)[0].visible; }

  [[nodiscard]] u8 dirty() const { return pool.dirties().at(id::index(id_)); }

  [[nodiscard]] render::Material const& material() const { return pool.material(id_); }

  void visibility(bool const visibility) const { pool.subMeshes(id_)[0].visible = visibility; }


  void diffuseColor(math::vec4 const color) const {
    pool.material(id_).base_color = color;
    setDirty();
  }

  void fresnel(math::vec3 const fresnel) const {
    pool.material(id_).fresnel = fresnel;
    setDirty();
  }

  void materialTransform(math::mat4 const transform) const {
    pool.material(id_).mat_transform = transform;
    setDirty();
  }

  void roughness(f32 const roughness) const {
    pool.material(id_).roughness = roughness;
    setDirty();
  }


  void unDirty() const {
    if (id_t const idx = id::index(id_); pool.dirties().at(idx) != 0) {
      --pool.dirties().at(idx);
    }
    else {
      pool.dirties().at(idx) = 0;
    }
  }

  void setDirty() const {
    if (dirty() == 3) {
      return;
    }
    if (dirty() == 0) {
      pool.dirtyIds().insert(id_);
    }
    pool.dirties().at(id::index(id_)) = 3;
  }

  enum Primitive : u8 { Cube = 0U, Plane, Cylinder, Sphere, Cone, Torus, Custom, count };

private:
  inline static GenericPool<pool_type>& pool = scene.componentPool<Geometry>();

  id_t id_;
};

template<>
inline void GenericPool<Geometry::pool_type>::update() {
  for (auto it = this->dirty_ids_.begin(); it != this->dirty_ids_.end();) {
    if (this->dirties_.at(id::index(*it)) == 0) {
      it = this->dirty_ids_.erase(it);
    }
    else {
      ++it;
    }
  }
}

} // namespace reveal3d::core
