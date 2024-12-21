/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file material.hpp
 * @version 1.0
 * @date 29/07/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"
#include "math/math.hpp"

namespace reveal3d::render {

struct Material {
  math::vec4 base_color {0.8F, 0.8F, 0.8F, 0.0F};
  math::vec3 fresnel {0.8F, 0.8F, 0.8F};
  f32 roughness {0.8F};
  math::mat4 transform {math::mat4_identity()};
};

} // namespace reveal3d::render
