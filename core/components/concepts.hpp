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

#include <type_traits>

namespace reveal3d::core {

template<typename T>
concept is_component = requires(T component) {
    { component.isAlive() } -> std::same_as<bool>;
    { component.id() } -> std::same_as<id_t>;
    component.data();
    typename T::InitInfo;
    typename T::Pool;
    typename T::Data;
};

template<typename T>
concept is_updatable = is_component<T> and requires(T component) {
    { component.update()  } -> std::same_as<void>;
};

template<typename T>
concept stored_in_gpu = is_component<T> and requires(T component) {
    { component.setDirty() } -> std::same_as<void>;
    { component.unDirty() } -> std::same_as<void>;
    { component.dirty() } -> std::same_as<u8>;
};

template<typename T>
concept component = is_component<T>;

}