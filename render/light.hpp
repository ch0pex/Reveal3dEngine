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
#include "vector.hpp"


namespace reveal3d::render {

class Light {
public:
  enum Type { directional, point, spot, count };

  explicit Light(id_t id);
  constexpr Light() = default;
  [[nodiscard]] constexpr bool IsValid() const { return id::is_valid(id_); }

  void isEnabled(bool is_enabled) const;
  void intensity(f32 intensity) const;
  void color(math::vec3 color) const;
  void attenuation(math::vec3 attenuation) const;
  void range(f32 range) const;
  void coneAngles(f32 umbra, f32 penumbra) const;

  [[nodiscard]] bool isEnabled() const;
  [[nodiscard]] f32 intensity() const;
  [[nodiscard]] math::vec3 color() const;
  [[nodiscard]] math::vec3 attenuation() const;
  [[nodiscard]] f32 range() const;
  [[nodiscard]] f32 umbra() const;
  [[nodiscard]] f32 penumbra() const;
  [[nodiscard]] Type type() const;
  [[nodiscard]] id_t id() const;

private:
  id_t id_ {id::invalid};
};

} // namespace reveal3d::render
