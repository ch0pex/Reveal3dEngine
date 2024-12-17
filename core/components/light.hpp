/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file light.hpp
 * @version 1.0
 * @date 14/12/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "component.hpp"

namespace reveal3d::core {

struct Light : Component<Light> {
  using pool_type = light::Pool;
  using init_info = pool_type::init_info;

  using Component::Component;

  void setDirty() const {
    if (dirty() == 4) {
      return;
    }
    if (dirty() == 0) {
      pool().dirtyIds().insert(id_);
    }
    pool().dirties().at(id::index(id_)) = 4;
  }

  [[nodiscard]] u8 dirty() const { return pool().dirties().at(id::index(id_)); }

  void active() const {
    pool().toggleActive(id_);
    setDirty();
  }

  void color(math::vec3 const color) const {
    pool().light(id_).color = color;
    setDirty();
  }

  void spotPower(f32 power) const {
    if (pool().lightType(id_) == render::Light::type::spot) {
      pool().light(id_).spot_power = power;
      setDirty();
    }
  }

  void direction(math::vec3 const direction) const {
    pool().light(id_).direction = direction;
    setDirty();
  }

  [[nodiscard]] bool isActive() const { pool().isActive(id_); }

  [[nodiscard]] render::Light::type type() const { return pool().lightType(id_); }
};

static_assert(component<Light>);

} // namespace reveal3d::core
