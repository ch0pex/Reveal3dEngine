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

template<typename T>
concept is_component = requires(T component) {
    { component.isAlive() } -> std::same_as<bool>;
    { component.id() } -> std::same_as<id_t>;
    component.data();
    typename T::Data;
};

template<typename T>
concept updatable = is_component<T> and requires(T component) {
    { component.update()  } -> std::same_as<void>;
};

template<typename T>
concept stored_in_gpu = std::same_as<std::true_type, typename T::stored_in_gpu>;

template<typename T>
concept component = is_component<T>;

template<typename T>
concept pool = requires(T pool) {
    { pool.count() } -> std::same_as<u32>;
    { pool.add(std::declval<id_t>(), std::declval<typename T::init_info&>()) } -> std::same_as<void>;
    { pool.remove(std::declval<id_t>()) } -> std::same_as<void>;
};


}