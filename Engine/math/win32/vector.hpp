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

class vec4;

using vec2 = XMFLOAT2;

class vec3 {
public:
    INLINE vec3() : vec_() {}
    INLINE vec3(const f32 x, const f32 y, const f32 z) : vec_(XMVectorSet(x, y, z, z)) {}
    INLINE vec3(const vec3 &vec) : vec_(vec) {}
    INLINE vec3(const scalar scalar) : vec_(XMVectorReplicate(scalar)) {}
    INLINE vec3(const XMFLOAT3 &vec) : vec_(XMLoadFloat3(&vec)) {}
    INLINE vec3(FXMVECTOR vec) : vec_(vec) {}
    INLINE operator XMVECTOR() const { return vec_; }

    INLINE void SetX(f32 x) { vec_ = XMVectorSetX(vec_, x); }
    INLINE void SetY(f32 y) { vec_ = XMVectorSetY(vec_, y); }
    INLINE void SetZ(f32 z) { vec_ = XMVectorSetZ(vec_, z); }
    [[nodiscard]] INLINE scalar GetX() const { return XMVectorGetX(vec_); }
    [[nodiscard]] INLINE scalar GetY() const { return XMVectorGetY(vec_); }
    [[nodiscard]] INLINE scalar GetZ() const { return XMVectorGetZ(vec_); }

    INLINE vec3 operator-() const { return XMVectorNegate(vec_); }
    INLINE vec3 operator+(vec3 v2) const { return XMVectorAdd(vec_, v2); }
    INLINE vec3 operator-(vec3 v2) const { return XMVectorSubtract(vec_, v2); }
    INLINE vec3 operator*(vec3 v2) const { return XMVectorMultiply(vec_, v2); }
    INLINE vec3 operator/(vec3 v2) const { return XMVectorDivide(vec_, v2); }
    INLINE vec3 operator*(const f32 v2) const { return *this * vec3(v2); }
    INLINE vec3 operator/(const f32 v2) const { return *this / vec3(v2); }

    INLINE void operator*=(const f32 v2) { *this = *this * vec3(v2); }
    INLINE void operator/=(const f32 v2) { *this = *this / vec3(v2); }

    INLINE friend vec3 operator* (scalar v1, vec3 v2) { return vec3(v1) * v2; }
    INLINE friend vec3 operator/ (scalar v1, vec3 v2) 	{ return vec3(v1) / v2; }
private:
    XMVECTOR vec_;
};

__declspec(align(16)) class vec4 {
public:
    INLINE vec4() : vec_() {}
    INLINE vec4(const f32 x, const f32 y, const f32 z, const f32 w) : vec_(XMVectorSet(x, y, z, w)) {}
    INLINE vec4(const vec4 &vec) : vec_(vec) {}
    INLINE vec4(scalar scalar) : vec_(XMVectorReplicate(scalar)) {}
    INLINE vec4(vec3 vec) : vec_() {}
    INLINE vec4(vec3 xyz, f32 w) : vec_(XMVectorSetW(xyz, w)) {}
    INLINE vec4(FXMVECTOR vec) : vec_(vec) {}
    INLINE vec4(const XMFLOAT3 &vec) : vec_(XMLoadFloat3(&vec)) {}

    INLINE void SetX(const f32 x) { vec_ = XMVectorSetX(vec_, x); }
    INLINE void SetY(const f32 y) { vec_ = XMVectorSetY(vec_, y); }
    INLINE void SetZ(const f32 z) { vec_ = XMVectorSetZ(vec_, z); }
    INLINE void SetW(const f32 w) { vec_ = XMVectorSetW(vec_, w); }
    INLINE void SetXYZ(vec3 xyz) { vec_ = XMVectorPermute<0,1,2,7>(xyz, vec_); }
    [[nodiscard]] INLINE scalar GetX() const { return XMVectorGetX(vec_); }
    [[nodiscard]] INLINE scalar GetY() const { return XMVectorGetY(vec_); }
    [[nodiscard]] INLINE scalar GetZ() const { return XMVectorGetZ(vec_); }
    [[nodiscard]] INLINE scalar GetW() const { return XMVectorGetW(vec_); }

    INLINE vec4 operator-() const { return XMVectorNegate(vec_); }
    INLINE vec4 operator+(vec4 v2) const { return XMVectorAdd(vec_, v2); }
    INLINE vec4 operator-(vec4 v2) const { return XMVectorSubtract(vec_, v2); }
    INLINE vec4 operator*(vec4 v2) const { return XMVectorMultiply(vec_, v2); }
    INLINE vec4 operator/(vec4 v2) const { return XMVectorDivide(vec_, v2); }
    INLINE vec4 operator*(scalar s) const { return *this * vec4(s); }
    INLINE vec4 operator/(scalar s) const { return *this / vec4(s); }
    INLINE void operator*=(const scalar s) { *this = *this * vec4(s); }
    INLINE void operator/=(const scalar s) { *this = *this / vec4(s); }
    INLINE operator DirectX::XMVECTOR() const { return vec_; }

    INLINE friend vec4 operator* (scalar v1, vec4 v2) { return vec4(v1) * v2; }
    INLINE friend vec4 operator/ (scalar v1, vec4 v2) 	{ return vec4(v1) / v2; }

private:
    XMVECTOR vec_;
};

class BoolVector
{
public:
    INLINE BoolVector( FXMVECTOR vec ) { m_vec = vec; }
    INLINE operator XMVECTOR() const { return m_vec; }
protected:
    XMVECTOR m_vec;
};

}


