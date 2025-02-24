/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file concepts.hpp
 * @version 1.0
 * @date 15/12/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/id.hpp"
#include "core/pooling/concepts.hpp"

namespace reveal3d::core {

namespace detail {

template<typename T>
concept is_component = requires(T component) {
  requires sizeof(T) == sizeof(id_t) + sizeof(void*) + sizeof(u32);
  requires pool_impl<typename T::pool_type>;
  typename T::init_info;
};

} // namespace detail

template<typename T>
concept component = detail::is_component<T>;

} // namespace reveal3d::core
