/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file pools_map.hpp
 * @version 1.0
 * @date 03/02/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <tuple>
#include "core/components/concepts.hpp"
#include "pool.hpp"

namespace reveal3d::core {

template<typename... Args>
struct PoolMap {
  template<component T>
  auto get() -> decltype(auto) {
    return (std::get<GenericPool<typename T::pool_type>>(data));
  }

  template<typename T>
  auto get() -> decltype(auto) {
    return (std::get<GenericPool<T>>(data));
  }

  std::tuple<GenericPool<Args>...> data;
};

} // namespace reveal3d::core
