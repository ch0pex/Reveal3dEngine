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
/*
namespace reveal3d::render {

namespace {

struct LightData {
  math::vec3 color; // Light color and strength
  math::vec3 direction; // Spot and directional only
  math::vec3 position; // Spot and point only
  f32 fall_off_start; // Spot and point only
  f32 fall_off_end; // Spot and point only
  f32 spot_power; // Spot only
};


std::vector<LightData> lights;

} // Anonymous namespace

Light::Light(const id_t id) : id_ {id} { id_t index {id::index(id)}; }

void Light::isEnabled(bool is_enabled) const { }

void Light::intensity(f32 intensity) const { }

void Light::color(math::vec3 color) const { }

void Light::attenuation(math::vec3 attenuation) const { }

void Light::range(f32 range) const { }

void Light::coneAngles(f32 umbra, f32 penumbra) const { }

bool Light::isEnabled() const { return false; }

f32 Light::intensity() const { return 0; }

math::vec3 Light::color() const { return reveal3d::math::vec3(); }

math::vec3 Light::attenuation() const { return reveal3d::math::vec3(); }

f32 Light::range() const { return 0; }

f32 Light::umbra() const { return 0; }
f32 Light::penumbra() const { return 0; }
Light::Type Light::type() const { return Light::point; }
id_t Light::id() const { return 0; }


} // namespace reveal3d::render
*/
