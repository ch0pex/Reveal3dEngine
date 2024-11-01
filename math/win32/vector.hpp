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
    inline xvec3() : vec_() {}
    inline xvec3(const f32 x, const f32 y, const f32 z) : vec_(XMVectorSet(x, y, z, z)) {}
    inline xvec3(const xvec3& vec) : vec_(vec) {}
    inline xvec3(const scalar scalar) : vec_(XMVectorReplicate(scalar)) {}
    inline xvec3(const XMFLOAT3& vec) : vec_(XMLoadFloat3(&vec)) {}
    inline xvec3(FXMVECTOR vec) : vec_(vec) {}
    inline operator XMVECTOR() const { return vec_; }

    inline void x(f32 x) { vec_ = XMVectorSetX(vec_, x); }
    inline void y(f32 y) { vec_ = XMVectorSetY(vec_, y); }
    inline void z(f32 z) { vec_ = XMVectorSetZ(vec_, z); }
    [[nodiscard]] inline scalar x() const { return XMVectorGetX(vec_); }
    [[nodiscard]] inline scalar y() const { return XMVectorGetY(vec_); }
    [[nodiscard]] inline scalar z() const { return XMVectorGetZ(vec_); }

    inline xvec3 operator-() const { return XMVectorNegate(vec_); }
    inline xvec3 operator+(xvec3 v2) const { return XMVectorAdd(vec_, v2); }
    inline xvec3 operator-(xvec3 v2) const { return XMVectorSubtract(vec_, v2); }
    inline xvec3 operator*(xvec3 v2) const { return XMVectorMultiply(vec_, v2); }
    inline xvec3 operator/(xvec3 v2) const { return XMVectorDivide(vec_, v2); }
    inline xvec3 operator*(const f32 v2) const { return *this * xvec3(v2); }
    inline xvec3 operator/(const f32 v2) const { return *this / xvec3(v2); }

    inline void operator+=(const xvec3 v2) { *this = XMVectorAdd(vec_, v2); }
    inline void operator-=(const xvec3 v2) { *this = XMVectorSubtract(vec_, v2); }
    inline void operator*=(const f32 v2) { *this = *this * xvec3(v2); }
    inline void operator/=(const f32 v2) { *this = *this / xvec3(v2); }

    inline friend xvec3 operator*(scalar v1, xvec3 v2) { return xvec3(v1) * v2; }
    inline friend xvec3 operator/(scalar v1, xvec3 v2) { return xvec3(v1) / v2; }

private:
    XMVECTOR vec_;
};

__declspec(align(16)) class xvec4 {
public:
    inline xvec4() : vec_() {}
    inline xvec4(const f32 x, const f32 y, const f32 z, const f32 w) : vec_(XMVectorSet(x, y, z, w)) {}
    inline xvec4(const xvec4& vec) : vec_(vec) {}
    inline xvec4(scalar scalar) : vec_(XMVectorReplicate(scalar)) {}
    inline xvec4(xvec3 vec) : vec_() {}
    inline xvec4(xvec3 xyz, f32 w) : vec_(XMVectorSetW(xyz, w)) {}
    inline xvec4(FXMVECTOR vec) : vec_(vec) {}
    inline xvec4(const XMFLOAT3& vec) : vec_(XMLoadFloat3(&vec)) {}

    inline void x(const f32 x) { vec_ = XMVectorSetX(vec_, x); }
    inline void y(const f32 y) { vec_ = XMVectorSetY(vec_, y); }
    inline void z(const f32 z) { vec_ = XMVectorSetZ(vec_, z); }
    inline void w(const f32 w) { vec_ = XMVectorSetW(vec_, w); }
    inline void xyz(xvec3 xyz) { vec_ = XMVectorPermute<0, 1, 2, 7>(xyz, vec_); }
    [[nodiscard]] inline scalar x() const { return XMVectorGetX(vec_); }
    [[nodiscard]] inline scalar y() const { return XMVectorGetY(vec_); }
    [[nodiscard]] inline scalar z() const { return XMVectorGetZ(vec_); }
    [[nodiscard]] inline scalar w() const { return XMVectorGetW(vec_); }

    inline xvec4 operator-() const { return XMVectorNegate(vec_); }
    inline xvec4 operator+(const xvec4 v2) const { return XMVectorAdd(vec_, v2); }
    inline xvec4 operator-(const xvec4 v2) const { return XMVectorSubtract(vec_, v2); }
    inline xvec4 operator*(const xvec4 v2) const { return XMVectorMultiply(vec_, v2); }
    inline xvec4 operator/(const xvec4 v2) const { return XMVectorDivide(vec_, v2); }
    inline xvec4 operator*(const scalar s) const { return *this * xvec4(s); }
    inline xvec4 operator/(const scalar s) const { return *this / xvec4(s); }
    inline void operator+=(const xvec4 v2) { *this = XMVectorAdd(vec_, v2); }
    inline void operator-=(const xvec4 v2) { *this = XMVectorSubtract(vec_, v2); }
    inline void operator+=(const scalar s) { *this = *this + xvec4(s); }
    inline void operator-=(const scalar s) { *this = *this - xvec4(s); }
    inline void operator*=(const scalar s) { *this = *this * xvec4(s); }
    inline void operator/=(const scalar s) { *this = *this / xvec4(s); }
    inline operator DirectX::XMVECTOR() const { return vec_; }
    inline operator xvec3() const { return vec_; }

    inline friend xvec4 operator*(scalar v1, xvec4 v2) { return xvec4(v1) * v2; }
    inline friend xvec4 operator/(scalar v1, xvec4 v2) { return xvec4(v1) / v2; }

private:
    XMVECTOR vec_;
};

class BoolVector {
public:
    inline BoolVector(FXMVECTOR vec) { m_vec_ = vec; }
    inline operator XMVECTOR() const { return m_vec_; }

protected:
    XMVECTOR m_vec_;
};

using vec2 = XMFLOAT2;
using vec3 = XMFLOAT3;
using vec4 = XMFLOAT4;

} // namespace reveal3d::math
