/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file rigidbody_pool.hpp
 * @version 1.0
 * @date 31/10/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "component.hpp"

namespace reveal3d::core {

struct Rigidbody : Component<Rigidbody> {
  using init_info = rigidbody::Pool::init_info;
  using pool_type = rigidbody::Pool;

  using Component::Component;

  // TODO
};

template<>
inline void GenericPool<Rigidbody::pool_type>::update(Scene& scene) {
  // TODO
}

} // namespace reveal3d::core
