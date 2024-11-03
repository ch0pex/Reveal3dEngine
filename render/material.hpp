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

enum Shader : u8 {
  Opaque = 0,
  Unlit, // Not affected by lighting
  Grid,
  //    Wireframe,

  count
};

struct Material {
  math::vec4 base_color {0.8f, 0.8f, 0.8f, 0.0f};
  math::vec3 fresnel {0.8f, 0.8f, 0.8f};
  f32 roughness {0.8f};
  math::mat4 mat_transform {math::mat4_identity()};
};

} // namespace reveal3d::render
