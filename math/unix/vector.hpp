/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vector.hpp
 * @version 1.0
 * @date 01/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "scalar.hpp"

namespace reveal3d::math {

template<u32 dim, u32 value>
concept Dimension = value >= dim;

template<u32 T>
  requires Dimension<2, T>
class vector {
public:
  vector() : vec_() { }
  template<u32 value = T>
    requires Dimension<3, value>
  vector(f32 const x, f32 const y, f32 const z) : vec_ {x, y, z} { }
  template<u32 value = T>
    requires Dimension<4, value>
  vector(f32 const x, f32 const y, f32 const z, f32 const w) : vec_ {x, y, z, w} { }
  vector(glm::vec<T, f32> const& vec) : vec_(vec) { }
  vector(scalar const scalar) : vec_(glm::vec3(scalar)) { }
  //    xvec3(const glm::vec3 &vec) : vec_(vec) {}
  template<u32 value = T>
    requires Dimension<3, value>
  vector(glm::vec3 vec) : vec_(vec) { }
  template<u32 value = T>
    requires Dimension<4, value>
  vector(glm::vec4 vec) : vec_(vec) { }
  template<u32 value = T>
    requires Dimension<3, value>
  operator glm::vec3() const {
    return vec_;
  }
  template<u32 value = T>
    requires Dimension<4, value>
  operator glm::vec4() const {
    return vec_;
  }

  void x(f32 x) { vec_.x = x; }
  void y(f32 y) { vec_.y = y; }
  template<u32 value = T>
    requires Dimension<3, value>
  void z(f32 z) {
    vec_.z = z;
  }
  template<u32 value = T>
    requires Dimension<4, value>
  void w(f32 w) {
    vec_.w = w;
  }

  [[nodiscard]] scalar x() const { return vec_.x; }
  [[nodiscard]] scalar y() const { return vec_.y; }
  template<u32 value = T>
    requires Dimension<3, value>
  [[nodiscard]] scalar z() const {
    return vec_.z;
  }
  template<u32 value = T>
    requires Dimension<4, value>
  [[nodiscard]] scalar w() const {
    return vec_.z;
  }

  vector operator-() const { return vec_; }
  vector operator+(vector v2) const { return vec_ + glm::vec<T, f32>(v2); }
  vector operator-(vector v2) const { return vec_ - glm::vec<T, f32>(v2); }
  vector operator*(vector v2) const { return vec_ * glm::vec<T, f32>(v2); }
  vector operator/(vector v2) const { return vec_ / glm::vec<T, f32>(v2); }
  vector operator*(f32 const v2) const { return *this * vector(v2); }
  vector operator/(f32 const v2) const { return *this / vector(v2); }

  void operator+=(vector const v2) { *this = *this + v2; }
  void operator-=(vector const v2) { *this = *this - v2; }
  void operator*=(f32 const v2) { *this = *this * v2; }
  void operator/=(f32 const v2) { *this = *this / v2; }

  friend vector operator*(scalar v1, vector v2) { return vector(v1) * v2; }
  friend vector operator/(scalar v1, vector v2) { return vector(v1) / v2; }

private:
  glm::vec<T, f32> vec_;
};


class BoolVector {
  // TODO
};


using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

using xvec3 = vector<3>;
using xvec4 = vector<4>;

} // namespace reveal3d::math
