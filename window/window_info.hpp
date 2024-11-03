/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file window_info.hpp
 * @version 1.0
 * @date 09/03/2024
 * @brief Window info
 *
 * Window initialization info
 */

#pragma once

#include "common/common.hpp"
#include "core/config/config.hpp"
#include "math/math.hpp"

namespace reveal3d::window {

using WCallback = uint8_t*;

struct Resolution {
  Resolution(const u32 width, const u32 height) :
    width(width), height(height), aspect_ratio(static_cast<f32>(width) / static_cast<f32>(height)) { }
  explicit Resolution(const math::vec2 res) :
    width(res.x), height(res.y), aspect_ratio(static_cast<f32>(width) / static_cast<f32>(height)) { }
  u32 width;
  u32 height;
  f32 aspect_ratio;
};


struct Info {
  const char* name {config::window.title.data()};
  Resolution res {config::window.resolution};
  WHandle handle {nullptr};
  WCallback callback {nullptr};
};

} // namespace reveal3d::window
