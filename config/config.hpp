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

#include <string_view>

namespace reveal3d::config {

namespace backends {

enum window { win32 = 0, glfw };

enum renderer { directx11 = 0, directx12, vulkan, openGl, metal };

inline renderer get_graphics_backend(std::string_view const name) {
  if (name == "directx11")
    return directx11;
  if (name == "directx12")
    return directx12;
  if (name == "vulkan")
    return vulkan;
  if (name == "openGl")
    return openGl;
  return directx12;
}

inline window get_window_backend(std::string_view const name) {
  if (name == "win32")
    return win32;
  if (name == "glfw")
    return glfw;
  return win32;
}

} // namespace backends

struct Backends {
  backends::window window {backends::win32};
  backends::renderer renderer {backends::directx12};
};

struct General { };

struct Scene {
  math::vec4 clearColor = {0.2F, 0.2F, 0.2F, 1.0F};
  bool showGrid         = true;
};

struct Render {
  struct Graphics {
    backends::renderer backend {backends::directx12};
    u8 max_framerate = 0;
    bool vsync       = true;
    u8 buffer_count  = 3;
    static constexpr u8 max_buffer_count {3};
  };
  struct Lighting {
    f32 ambient_light_intensity    = 0.7F;
    f32 sun_light_intensity        = 0.9F;
    math::vec3 ambient_color       = {1.0F, 1.0F, 1.0F};
    math::vec3 sun_light_color     = {1.0F, 1.0F, 1.0F};
    math::vec3 sun_light_direction = {0.0F, 0.0F, -1.0F};
  };

  Graphics graphics;
  Lighting lighting;
};

struct Window {
  backends::window backend {backends::win32};
  std::string_view title = "Reveal3D";
  math::vec2 resolution  = {1920, 1080};
};

struct Camera {
  f32 fov            = 45.0F;
  f32 near_plane     = 0.1F;
  f32 far_plane      = 25.0F;
  f32 movement_speed = 5.0F;
  f32 sensitivity    = 50.0F * 0.001F;
  static constexpr math::vec3 world_up {0, 0, 1};
};

inline General general = {};
inline Scene scene     = {};
inline Render render   = {};
inline Window window   = {};
inline Camera camera   = {};

} // namespace reveal3d::config
