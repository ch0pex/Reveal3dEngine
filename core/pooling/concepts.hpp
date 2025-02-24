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

#include "common/common.hpp"

#include <concepts>
#include <type_traits>

namespace reveal3d::core {

template<typename T>
concept pool_impl = requires(T pool) {
  typename T::gpu_stored;
  typename T::init_info;
  { pool.countData() } -> std::same_as<index_t>;
  { pool.addData(std::declval<typename T::init_info const&>()) } -> std::same_as<void>;
  { pool.removeData(std::declval<id_t>()) } -> std::same_as<void>;
};

template<typename T>
concept stored_in_gpu = std::same_as<std::true_type, typename T::gpu_stored>;

} // namespace reveal3d::core
