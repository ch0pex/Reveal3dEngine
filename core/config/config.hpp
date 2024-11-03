/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file config.hpp
 * @version 1.0
 * @date 21/03/2024
 * @brief Config engine structs
 *
 * All this structs can be used to configure the system
 */

#pragma once

#include "math/math.hpp"

namespace reveal3d::config {

struct Scene {
  math::vec4 clearColor = {0.2F, 0.2F, 0.2F, 1.0F};
  bool showGrid         = true;
};

struct Graphics {
  u8 max_framerate = 0;
  bool vsync       = true;
  static constexpr u8 buffer_count {3};
};

struct Lighting {
  f32 ambient_light_intensity    = 0.7F;
  f32 sun_light_intensity        = 0.9F;
  math::vec3 ambient_color       = {1.0F, 1.0F, 1.0F};
  math::vec3 sun_light_color     = {1.0F, 1.0F, 1.0F};
  math::vec3 sun_light_direction = {0.0F, 0.0F, -1.0F};
};

struct Window {
  std::string_view title = "Reveal3D";
  math::vec2 resolution  = {1920, 1080};
};

struct Camera {
  f32 fov            = 45.0F;
  f32 near_plane     = 0.1F;
  f32 far_plane      = 100.0F;
  f32 movement_speed = 5.0F;
  f32 sensitivity    = 50.0F * 0.001F;
  static constexpr math::vec3 world_up {0, 0, 1};
};

inline Scene scene       = {};
inline Graphics graphics = {};
inline Lighting lighting = {};
inline Window window     = {};
inline Camera camera     = {};

} // namespace reveal3d::config
