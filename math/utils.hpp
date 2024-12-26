/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file utils.hpp
 * @version 1.0
 * @date 01/11/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "vector.hpp"

#include <concepts>
#include <type_traits>

namespace reveal3d::math::utl {

inline auto to_array(xvec3 v) { return std::array {v.x(), v.y(), v.z()}; }
inline auto to_array(xvec4 v) { return std::array {v.x(), v.y(), v.z(), v.w()}; }
inline auto to_array(vec3 v) { return std::array {v.x, v.y, v.z}; }
inline auto to_array(vec4 v) { return std::array {v.x, v.y, v.z, v.w}; }

} // namespace reveal3d::math::utl
