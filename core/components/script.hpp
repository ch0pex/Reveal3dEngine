/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file script.hpp
 * @version 1.0
 * @date 01/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once
#include "core/scene.hpp"

#include "component.hpp"

namespace reveal3d::core {

struct Script : Component<Script> {
  using init_info = script::Pool::init_info;
  using pool_type = script::Pool;
  using Component::Component;

  [[nodiscard]] constexpr bool isAlive() const { return id_ != id::invalid; }

  [[nodiscard]] constexpr id_t id() const { return id_; }

  void begin() { }

  void disableUpdate() { }

  void enableUpdate() { }

  void destroyed() { }

  void update() { }

  void data();

  void enableBegin();

  void disableBegin();

  void enableDestroyed();

  void disableDestroyed();
};

template<>
inline void GenericPool<Script::pool_type>::update() {
  // TODO
}

} // namespace reveal3d::core
