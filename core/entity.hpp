/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file entity.hpp
 * @version 1.0
 * @date 06/02/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "components/concepts.hpp"
#include "scene.hpp"

namespace reveal3d::core {

class Entity {
public:
  Entity() : id_(id::invalid) { }

  Entity(id_t const id) : id_ {id} {};

  template<detail::is_component T>
  T component() const {
    if (not isAlive()) {
      return T();
    }
    return scene.pool<T>().at(id_);
  }
  template<detail::is_component T>
  T addComponent() {
    return addComponent<T>({});
  }

  template<detail::is_component T>
  T addComponent(typename T::init_info const& init_info) {
    if (not isAlive()) {
      return T();
    }
    return scene.pool<T>().addComponent(id_, init_info);
  }

  template<detail::is_component T>
  void removeComponent() {
    if (isAlive()) {
      scene.pool<T>().removeComponent(id_);
    }
  }

  [[nodiscard]] u32 id() const { return id_; }

  [[nodiscard]] bool isAlive() const { return scene.isAlive(id_); }

  Entity addChild() const { return scene.newChildEntity(id_); }

  Entity parent() const { return scene.getNode(id_).parent; }

  Entity next() const { return scene.getNode(id_).next; }

  Entity prev() const { return scene.getNode(id_).prev; }

  Entity firstChild() const { return scene.getNode(id_).first_child; }

  auto children() const { return scene.getNode(id_).getChildren(); }

  bool operator==(Entity const& other) const { return id_ == other.id_; }

  operator id_t() const { return id_; }

private:
  id_t id_;
};

} // namespace reveal3d::core
