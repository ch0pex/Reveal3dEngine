/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file light.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"
#include "math/math.hpp"


namespace reveal3d::render {

enum class LightType : u8 { directional = 0, point, spot, count };

struct Light {
  using type = LightType;

  math::vec3 color; // Light color and strength
  math::vec3 position; // Spot and point only
  math::vec3 direction; // Spot and directional only
  f32 fall_off_start; // Spot and point only
  f32 fall_off_end; // Spot and point only
  f32 spot_power; // Spot only
  bool active {true};
};

} // namespace reveal3d::render
