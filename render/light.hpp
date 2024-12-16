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

struct Light {
  enum class Type : u8 { directional, point, spot, count };
  using type = Type;

  math::vec3 color; // Light color and strength
  math::vec3 position; // Spot and point only
  math::vec3 direction; // Spot and directional only
  f32 fall_off_start; // Spot and point only
  f32 fall_off_end; // Spot and point only
  f32 spot_power; // Spot only
};

/*
struct SpotLight : Light {
  math::vec3 position; // Spot and point only
  math::vec3 direction; // Spot and directional only
  f32 fall_off_start; // Spot and point only
  f32 fall_off_end; // Spot and point only
  f32 spot_power; // Spot only
};

struct PointLight : Light {
  math::vec3 position; // Spot and point only
  f32 fall_off_start; // Spot and point only
  f32 fall_off_end; // Spot and point only
};

struct DirectionalLight : Light {
  math::vec3 direction; // Spot and directional only
};
*/

} // namespace reveal3d::render
