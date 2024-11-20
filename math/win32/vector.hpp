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

class xvec4;

class xvec3 {
public:
  xvec3() : vec_() { }
  xvec3(f32 const x, f32 const y, f32 const z) : vec_(XMVectorSet(x, y, z, z)) { }
  xvec3(xvec3 const& vec) : vec_(vec) { }
  xvec3(scalar const scalar) : vec_(XMVectorReplicate(scalar)) { }
  xvec3(const XMFLOAT3& vec) : vec_(XMLoadFloat3(&vec)) { }
  xvec3(FXMVECTOR vec) : vec_(vec) { }
  operator XMVECTOR() const { return vec_; }

  void x(f32 x) { vec_ = XMVectorSetX(vec_, x); }
  void y(f32 y) { vec_ = XMVectorSetY(vec_, y); }
  void z(f32 z) { vec_ = XMVectorSetZ(vec_, z); }
  [[nodiscard]] auto x() const { return XMVectorGetX(vec_); }
  [[nodiscard]] auto y() const { return XMVectorGetY(vec_); }
  [[nodiscard]] auto z() const { return XMVectorGetZ(vec_); }

  xvec3 operator-() const { return XMVectorNegate(vec_); }
  xvec3 operator+(xvec3 v2) const { return XMVectorAdd(vec_, v2); }
  xvec3 operator-(xvec3 v2) const { return XMVectorSubtract(vec_, v2); }
  xvec3 operator*(xvec3 v2) const { return XMVectorMultiply(vec_, v2); }
  xvec3 operator/(xvec3 v2) const { return XMVectorDivide(vec_, v2); }
  xvec3 operator*(f32 const v2) const { return *this * xvec3(v2); }
  xvec3 operator/(f32 const v2) const { return *this / xvec3(v2); }

  void operator+=(xvec3 const v2) { *this = XMVectorAdd(vec_, v2); }
  void operator-=(xvec3 const v2) { *this = XMVectorSubtract(vec_, v2); }
  void operator*=(f32 const v2) { *this = *this * xvec3(v2); }
  void operator/=(f32 const v2) { *this = *this / xvec3(v2); }
  bool operator==(xvec3 const v2) const { return XMVector3Equal(vec_, v2.vec_); }

  friend xvec3 operator*(scalar v1, xvec3 v2) { return xvec3(v1) * v2; }
  friend xvec3 operator/(scalar v1, xvec3 v2) { return xvec3(v1) / v2; }

private:
  XMVECTOR vec_;
};

class alignas(16) xvec4 {
public:
  xvec4() : vec_() { }
  xvec4(f32 const x, f32 const y, f32 const z, f32 const w) : vec_(XMVectorSet(x, y, z, w)) { }
  xvec4(xvec4 const& vec) : vec_(vec) { }
  xvec4(scalar scalar) : vec_(XMVectorReplicate(scalar)) { }
  xvec4(xvec3 vec) : vec_() { }
  xvec4(xvec3 xyz, f32 w) : vec_(XMVectorSetW(xyz, w)) { }
  xvec4(FXMVECTOR vec) : vec_(vec) { }
  xvec4(const XMFLOAT3& vec) : vec_(XMLoadFloat3(&vec)) { }

  void x(f32 const x) { vec_ = XMVectorSetX(vec_, x); }
  void y(f32 const y) { vec_ = XMVectorSetY(vec_, y); }
  void z(f32 const z) { vec_ = XMVectorSetZ(vec_, z); }
  void w(f32 const w) { vec_ = XMVectorSetW(vec_, w); }
  void xyz(xvec3 xyz) { vec_ = XMVectorPermute<0, 1, 2, 7>(xyz, vec_); }
  [[nodiscard]] scalar x() const { return XMVectorGetX(vec_); }
  [[nodiscard]] scalar y() const { return XMVectorGetY(vec_); }
  [[nodiscard]] scalar z() const { return XMVectorGetZ(vec_); }
  [[nodiscard]] scalar w() const { return XMVectorGetW(vec_); }

  xvec4 operator-() const { return XMVectorNegate(vec_); }
  xvec4 operator+(xvec4 const v2) const { return XMVectorAdd(vec_, v2); }
  xvec4 operator-(xvec4 const v2) const { return XMVectorSubtract(vec_, v2); }
  xvec4 operator*(xvec4 const v2) const { return XMVectorMultiply(vec_, v2); }
  xvec4 operator/(xvec4 const v2) const { return XMVectorDivide(vec_, v2); }
  xvec4 operator*(scalar const s) const { return *this * xvec4(s); }
  xvec4 operator/(scalar const s) const { return *this / xvec4(s); }
  void operator+=(xvec4 const v2) { *this = XMVectorAdd(vec_, v2); }
  void operator-=(xvec4 const v2) { *this = XMVectorSubtract(vec_, v2); }
  void operator+=(scalar const s) { *this = *this + xvec4(s); }
  void operator-=(scalar const s) { *this = *this - xvec4(s); }
  void operator*=(scalar const s) { *this = *this * xvec4(s); }
  void operator/=(scalar const s) { *this = *this / xvec4(s); }
  operator XMVECTOR() const { return vec_; }
  operator xvec3() const { return vec_; }
  bool operator==(xvec4 const v2) const { return XMVector4Equal(vec_, v2.vec_); }

  friend xvec4 operator*(scalar v1, xvec4 v2) { return xvec4(v1) * v2; }
  friend xvec4 operator/(scalar v1, xvec4 v2) { return xvec4(v1) / v2; }

private:
  XMVECTOR vec_;
};

class BoolVector {
public:
  BoolVector(FXMVECTOR vec) { m_vec_ = vec; }
  operator XMVECTOR() const { return m_vec_; }

protected:
  XMVECTOR m_vec_;
};


using vec2 = XMFLOAT2;
using vec3 = XMFLOAT3;
using vec4 = XMFLOAT4;


} // namespace reveal3d::math
