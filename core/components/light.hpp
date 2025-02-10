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

  void color(math::vec3 const color) const {
    pool().light(id_).color = color;
    setDirty();
  }

  void position(math::vec3 const position) const {
    if (pool().lightType(id_) != render::Light::type::directional) {
      pool().light(id_).position = position;
      setDirty();
    }
    else {
      logger(LogWarning) << "Setting position property to a directional light, this action will be ignored";
    }
  }

  void direction(math::vec3 const direction) const {
    if (pool().lightType(id_) == render::Light::type::directional) {
      pool().light(id_).direction = direction;
      setDirty();
    }
    else {
      logger(LogWarning) << "Setting direction property to a non directional light, this action will be ignored";
    }
  }

  void fallOffStart(f32 const power) const {
    if (pool().lightType(id_) != render::Light::type::directional) {
      pool().light(id_).spot_power = power;
      setDirty();
    }
    else {
      logger(LogWarning) << "Setting fall off start property to a directional light, this action will be ignored";
    }
  }

  void fallOffEnd(f32 const power) const {
    if (pool().lightType(id_) != render::Light::type::directional) {
      pool().light(id_).spot_power = power;
      setDirty();
    }
    else {
      logger(LogWarning) << "Setting fall off end property to a directional light, this action will be ignored";
    }
  }

  void spotPower(f32 const power) const {
    if (pool().lightType(id_) == render::Light::type::spot) {
      pool().light(id_).spot_power = power;
      setDirty();
    }
    else {
      logger(LogWarning) << "Setting spot power property to a non stop light, this action will be ignored";
    }
  }

  void active(bool act) const {
    pool().light(id_).active = act;
    setDirty();
  }

  bool active() const { return pool().light(id_).active; }

  render::Light const& properties() const { return pool().light(id_); }

  [[nodiscard]] render::Light::type type() const { return pool().lightType(id_); }
};

template<>
inline void GenericPool<Light::pool_type>::update(Scene& scene) {
  // TODO
}

static_assert(component<Light>);
} // namespace reveal3d::core
