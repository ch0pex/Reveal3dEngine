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
  Entity() : scene_(nullptr), id_(id::invalid) { }

  explicit Entity(Scene* scene) : scene_(scene), id_(id::invalid) { }

  Entity(Scene* scene, id_t const id) : id_ {id}, scene_(scene) {};

  template<detail::is_component T>
  T component() const {
    if (not isAlive()) {
      return T(scene_);
    }
    return {scene_, scene_->pool<T>().at(id_)};
  }
  template<detail::is_component T>
  T addComponent() {
    return addComponent<T>({});
  }

  template<detail::is_component T>
  T addComponent(typename T::init_info const& init_info) {
    if (not isAlive()) {
      return T(scene_);
    }
    return {scene_, scene_->pool<T>().addComponent(id_, init_info)};
  }

  template<detail::is_component T>
  void removeComponent() {
    if (isAlive()) {
      scene_->pool<T>().removeComponent(id_);
    }
  }

  [[nodiscard]] u32 id() const { return id_; }

  [[nodiscard]] bool isAlive() const { return scene_->isAlive(id_); }

  Entity addChild() const { return {scene_, scene_->newChildEntity(id_)}; }

  [[nodiscard]] Entity parent() const { return {scene_, scene_->node(id_).parent}; }

  [[nodiscard]] Entity next() const { return {scene_, scene_->node(id_).next}; }

  [[nodiscard]] Entity prev() const { return {scene_, scene_->node(id_).prev}; }

  [[nodiscard]] Entity firstChild() const { return {scene_, scene_->node(id_).first_child}; }

  [[nodiscard]] auto children() const {
    std::vector<Entity> children;
    if (firstChild().isAlive()) {
      Entity current = firstChild();
      while (true) {
        children.push_back(current);
        if (current.next().isAlive()) {
          current = current.next();
        }
        else {
          break;
        }
      }
    }
    return children;
  }

  bool operator==(Entity const& other) const { return id_ == other.id_; }

  operator id_t() const { return id_; }

private:
  Scene* scene_;
  id_t id_;

  u32 flags_; // TODO maybe some flags here?
};

inline Entity new_entity(Scene& scene) { return {&scene, scene.newEntity()}; }

} // namespace reveal3d::core
