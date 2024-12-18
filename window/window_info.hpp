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
#include "config/config.hpp"
#include "math/math.hpp"

namespace reveal3d::window {

using WCallback = uint8_t*;

struct Resolution {
  constexpr Resolution(u32 const width, u32 const height) : width(width), height(height) { }

  explicit constexpr Resolution(math::vec2 const res) : width(res.x), height(res.y) { }

  [[nodiscard]] constexpr f32 aspect_ratio() const { return static_cast<f32>(width) / static_cast<f32>(height); }

  [[nodiscard]] constexpr bool isNull() const { return width == 0 or height == 0; }

  constexpr bool operator==(Resolution const other) const { return width == other.width && height == other.height; }

  u32 width;
  u32 height;
};

struct Info {
  char const* name {config::window.title.data()};
  Resolution res {config::window.resolution};
  WHandle handle {nullptr};
  WCallback callback {nullptr};
};

} // namespace reveal3d::window
