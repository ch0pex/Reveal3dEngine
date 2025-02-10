/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file component.hpp
 * @version 1.0
 * @date 15/12/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once


#include "core/entity.hpp"
#include "core/scene.hpp"

namespace reveal3d::core {

template<typename T>
class Component {
public:
  constexpr Component(Scene* scene) : id_(id::invalid), scene_(scene) { }

  constexpr Component(Scene* scene, id_t const id) : id_(id), scene_(scene) { }

  [[nodiscard]] constexpr bool isAlive() const { return id_ != id::invalid; }

  [[nodiscard]] constexpr id_t id() const { return id_; }

  [[nodiscard]] Entity entity() const { return {scene_, scene_->entity(pool().getMappedId(id_))}; }

  [[nodiscard]] u8 dirty() const { return pool().dirties().at(id::index(id_)); }

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
    pool().dirties().at(id::index(id_)) = config::render.graphics.buffer_count;
  }

  auto pool() const -> decltype(auto) { return scene_->pool<T>(); };

protected:
  id_t id_;
  Scene* scene_;
};


} // namespace reveal3d::core
