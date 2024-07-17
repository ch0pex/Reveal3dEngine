/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file components.hpp
 * @version 1.0
 * @date 16/07/2024
 * @brief Components
 *
 * Components includes
 */

#pragma once

#include "components/transform.hpp"
#include "components/geometry.hpp"
#include "components/script.hpp"
#include "math/math.hpp"

namespace reveal3d::core {


template<typename T>
concept component = requires(T component) {
    typename T::PoolType;
    {component.IsAlive()} -> std::same_as<bool>;
    {component.Id()} -> std::same_as<id_t>;
};

}
