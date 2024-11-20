/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file transform.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "core/scene.hpp"

#include <set>
#include <vector>

namespace reveal3d::core {

class Transform {
public:
  // *** Type Traits ***

  using pool_type     = transform::Pool;
  using stored_in_gpu = std::true_type;

  // *** Constructors ***

  constexpr Transform() = default;

  constexpr Transform(id_t const id) : id_(id) { }

  // *** Member methods ***

  [[nodiscard]] constexpr bool isAlive() const { return id_ != id::invalid; }

  [[nodiscard]] constexpr id_t id() const { return id_; }

  [[nodiscard]] u32 entityIdx() const { return pool.getMappedId(id_); }

  [[nodiscard]] math::mat4& world() const { return pool.world(id_); }

  [[nodiscard]] math::mat4& invWorld() const { return pool.invWorld(id_); }

  [[nodiscard]] math::xvec3 position() const { return pool.posRotScale(id_).position; }

  [[nodiscard]] math::xvec3 scale() const { return pool.posRotScale(id_).scale; }

  [[nodiscard]] math::xvec3 rotation() const { return vec_to_degrees(pool.posRotScale(id_).rotation); }

  [[nodiscard]] math::xvec3 worldPosition() const {
    math::mat4 const world_mat = pool.world(id_);
    return world_mat.translation();
  }

  [[nodiscard]] math::xvec3 worldScale() const { return pool.world(id_).scale(); }

  [[nodiscard]] math::xvec3 worldRotation() const { return vec_to_degrees(pool.world(id_).rotation()); }

  void position(math::xvec3 pos) const {
    pool.posRotScale(id_).position = pos;
    setDirty();
  }

  void scale(math::xvec3 const size) const {
    pool.posRotScale(id_).scale = size;
    setDirty();
  }

  void rotation(math::xvec3 const rot) const {
    pool.posRotScale(id_).rotation = vec_to_radians(rot);
    setDirty();
  }

  void rotation(math::xvec4 const rot) const {
    pool.posRotScale(id_).rotation = vec_to_radians(rot);
    setDirty();
  }

  void worldPosition(math::xvec3 const pos) const {
    transform::detail::Transform& trans = pool.posRotScale(id_);
    pool.world(id_)                     = transpose(affine_transformation(pos, trans.scale, trans.rotation));
    if (Entity const parent = scene.getNode(id_).parent; parent.isAlive()) {
      trans.position = transpose(parent.component<Transform>().invWorld()) * pos;
    }
    else {
      trans.position = pos;
    }

    pool.invWorld(id_) = inverse(pool.world(id_));
    softDirty();
  }

  void worldScale(math::xvec3 const scale) const {
    transform::detail::Transform& trans = pool.posRotScale(id_);
    pool.world(id_)                     = transpose(affine_transformation(trans.position, scale, trans.rotation));
    if (Entity const parent = scene.getNode(id_).parent; parent.isAlive()) {
      trans.scale = parent.component<Transform>().invWorld() * scale;
    }
    else {
      trans.scale = scale;
    }
    pool.invWorld(id_) = inverse(pool.world(id_));
    softDirty();
  }

  void worldRotation(math::xvec3 const rot) const {
    transform::detail::Transform& trans = pool.posRotScale(id_);

    auto const rad = vec_to_radians(rot);
    world()        = transpose(affine_transformation(trans.position, trans.scale, rad));
    if (Entity const parent = scene.getNode(id_).parent; parent.isAlive()) {
      trans.rotation = parent.component<Transform>().invWorld() * rad;
    }
    else {
      trans.rotation = rad;
    }
    pool.invWorld(id_) = inverse(pool.world(id_));
    softDirty();
  }

  void update() const {
    if (pool.dirties().at(id::index(id_)) != 4) {
      return;
    }

    if (Scene::Node const& curr_node = scene.getNode(id::index(id_)); curr_node.parent.isAlive()) {
      id_t const parent_id = curr_node.parent.id();
      curr_node.parent.component<Transform>().update();
      math::mat4 const parent_world = pool.world(parent_id);
      world()                       = parent_world * calcWorld(id_);
    }
    else {
      world() = calcWorld(id_);
    }
    invWorld() = transpose(inverse(world()));
    --pool.dirties().at(id::index(id_));
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
    if (dirty() == 4) {
      return;
    }
    if (dirty() == 0) {
      pool.dirtyIds().insert(id_);
    }
    setChildrenAsDirty();
    pool.dirties().at(id::index(id_)) = 4;
  }

  [[nodiscard]] u8 dirty() const { return pool.dirties().at(id::index(id_)); }

private:
  static math::mat4 calcWorld(id_t const id) {
    auto& [position, rotation, scale] = scene.componentPool<Transform>().posRotScale(id);
    return transpose(affine_transformation(position, scale, rotation));
  }

  void softDirty() const {
    pool.dirties().at(id::index(id_)) = 3;
    pool.dirtyIds().insert(id_);
    setChildrenAsDirty();
  }

  void setChildrenAsDirty() const {
    Scene::Node const& node = scene.getNode(id_);
    for (auto const children = node.getChildren(); auto const child_id: children) {
      auto const child = Entity(child_id);
      child.component<Transform>().setDirty();
      child.component<Transform>().setChildrenAsDirty();
    }
  }

  inline static GenericPool<transform::Pool>& pool = scene.componentPool<Transform>();

  id_t id_ {id::invalid};
};

template<>
inline void GenericPool<Transform::pool_type>::update() {
  for (auto it = this->dirty_ids_.begin(); it != this->dirty_ids_.end();) {
    Transform component {*it};
    if constexpr (updatable<Transform>) {
      component.update();
    }
    if (this->dirties_.at(id::index(*it)) == 0) {
      it = this->dirty_ids_.erase(it);
    }
    else {
      ++it;
    }
  }
}

} // namespace reveal3d::core
