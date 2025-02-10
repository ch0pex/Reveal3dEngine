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

#include <concepts>

namespace reveal3d::core {

namespace detail {

template<typename T>
concept is_component = requires(T component) {
  { sizeof(T) == 8 };
  typename T::pool_type;
  typename T::init_info;
};

} // namespace detail

template<typename T>
concept component = detail::is_component<T>;

} // namespace reveal3d::core
