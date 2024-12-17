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

#include "component.hpp"

#include <set>
#include <span>

namespace reveal3d::core {

struct Geometry : Component<Geometry> {
  using pool_type = geometry::Pool;
  using init_info = pool_type::init_info;

  enum Primitive : u8 { Cube = 0U, Plane, Cylinder, Sphere, Cone, Torus, Custom, count };

  using Component::Component;

  constexpr Geometry(Geometry const& other) : Component {other.id()} { }

  constexpr Geometry(Geometry&& other) noexcept : Component {other.id()} { }

  Geometry& operator=(Geometry const& other) = default;

  Geometry& operator=(Geometry&& other) noexcept {
    id_ = other.id_;
    return *this;
  }

  // Note: for now we only will have one mesh and submesh, so we move the vector instead of appending it
  void addMesh(render::Mesh& mesh) const {
    render::SubMesh sub_mesh;
    pool().mesh(id_) = std::move(mesh);

    sub_mesh.vertex_pos  = vertices().size();
    sub_mesh.index_pos   = 0;
    sub_mesh.index_count = indexCount();
  }

  [[nodiscard]] u32 triangles() const { return pool().mesh(id_).triangle_count; }

  [[nodiscard]] u32 vertexCount() const { return pool().mesh(id_).vertex_count; }

  [[nodiscard]] u32 indexCount() const { return pool().mesh(id_).indices.size(); }

  [[nodiscard]] std::span<render::SubMesh> subMeshes() const { return pool().subMeshes(id_); }

  [[nodiscard]] std::span<render::Vertex> vertices() const { return pool().mesh(id_).vertices; }

  [[nodiscard]] std::span<u32> indices() const { return pool().mesh(id_).indices; }

  [[nodiscard]] bool isVisible() const { return pool().subMeshes(id_)[0].visible; }

  [[nodiscard]] u8 dirty() const { return pool().dirties().at(id::index(id_)); }

  [[nodiscard]] render::Material const& material() const { return pool().material(id_); }

  void visibility(bool const visibility) const { pool().subMeshes(id_)[0].visible = visibility; }


  void diffuseColor(math::vec4 const color) const {
    pool().material(id_).base_color = color;
    setDirty();
  }

  void fresnel(math::vec3 const fresnel) const {
    pool().material(id_).fresnel = fresnel;
    setDirty();
  }

  void materialTransform(math::mat4 const& transform) const {
    pool().material(id_).transform = transform;
    setDirty();
  }

  void roughness(f32 const roughness) const {
    pool().material(id_).roughness = roughness;
    setDirty();
  }


  void unDirty() const {
    if (id_t const idx = id::index(id_); pool().dirties().at(idx) != 0) {
      --pool().dirties().at(idx);
    }
    else {
      pool().dirties().at(idx) = 0;
    }
  }

  void setDirty() const {
    if (dirty() == 3) {
      return;
    }
    if (dirty() == 0) {
      pool().dirtyIds().insert(id_);
    }
    pool().dirties().at(id::index(id_)) = 3;
  }
};
static_assert(component<Geometry>);

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
