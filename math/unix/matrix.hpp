/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file matrix.hpp
 * @version 1.0
 * @date 01/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once


#include "vector.hpp"

namespace reveal3d::math {

template<u32 T>
class matrix {
public:
  matrix() : mat_() { }
  matrix(f32 const* data) { }
  matrix(glm::mat4 const& mat) : mat_(mat) { }
  matrix(matrix const& mat) : mat_(mat.mat_) { }

  [[nodiscard]] matrix const& get3x3() const { return (matrix const&)*this; }
  [[nodiscard]] vector<T> x() const { return mat_[0]; }
  [[nodiscard]] vector<T> y() const { return mat_[1]; }
  [[nodiscard]] vector<T> z() const { return mat_[2]; }
  [[nodiscard]] vector<T> w() const { return mat_[3]; }
  void x(vector<T> x) { mat_.r[0] = x; }
  void y(vector<T> y) { mat_.r[1] = y; }
  void z(vector<T> z) { mat_.r[2] = z; }
  void w(vector<T> w) { mat_.r[3] = w; }


  operator glm::mat<T, T, f32>() const { return mat_; }
  //    xvec4 operator*(xvec3 vec) const { return vec * mat_; }
  //    xvec4 operator*(xvec4 vec) const { return vec * mat_; }
  matrix operator*(matrix const& mat) const { return mat.mat_ * mat_; }
  matrix MakeScale(scalar scale) { return scale * mat_; }
  //    matrix MakeScale(xvec3 scale) { return XMMatrixScalingFromVector(scale); }
private:
  glm::mat<T, T, f32> mat_;
};


using mat3 = matrix<3>;
using mat4 = matrix<4>;

} // namespace reveal3d::math
