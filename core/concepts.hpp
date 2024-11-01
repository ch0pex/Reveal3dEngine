/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file concepts.hpp
 * @version 1.0
 * @date 25/10/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"

#include <concepts>
#include <type_traits>

namespace reveal3d::core {

namespace detail {

template<typename T>
concept is_component = requires(T component) {
    { T{} };
    { T{}.id() == id::invalid };
    { component.isAlive() } -> std::same_as<bool>;
    { component.id() } -> std::same_as<id_t>;
    { std::is_constructible_v<T, id_t> };
    { sizeof(T) == 4 };
};

} // namespace detail

template<typename T>
concept updatable = detail::is_component<T> and requires(T component) {
    { component.update() } -> std::same_as<void>;
};

template<typename T>
concept stored_in_gpu = std::same_as<std::true_type, typename T::stored_in_gpu>;

template<typename T>
concept component = detail::is_component<T>;

template<typename T>
concept pool = requires(T pool) {
    { pool.countData() } -> std::same_as<u32>;
    { pool.addData(std::declval<id_t>(), std::declval<typename T::init_info&>()) } -> std::same_as<void>;
    { pool.removeData(std::declval<id_t>()) } -> std::same_as<void>;
};


} // namespace reveal3d::core
