/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file constants.hpp
 * @version 1.0
 * @date 11/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "math/math.hpp"

namespace reveal3d::graphics {

struct GlobalShaderData {
  math::mat4 view;
  math::mat4 inv_view;
  math::mat4 proj;
  math::mat4 inv_proj;
  math::mat4 view_proj;
  math::mat4 inv_view_proj;
  math::vec3 eye_pos;
  f32 near_z;
  f32 far_z;
  math::vec2 render_target_size;
  f32 total_time;
  f32 delta_time;
};

struct PerObjectData {
  math::mat4 world_view_proj;
  u32 entity_id;
};


} // namespace reveal3d::graphics
