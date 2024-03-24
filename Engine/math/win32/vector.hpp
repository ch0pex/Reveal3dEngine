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
    INLINE xvec3() : vec_() {}
    INLINE xvec3(const f32 x, const f32 y, const f32 z) : vec_(XMVectorSet(x, y, z, z)) {}
    INLINE xvec3(const xvec3 &vec) : vec_(vec) {}
    INLINE xvec3(const scalar scalar) : vec_(XMVectorReplicate(scalar)) {}
    INLINE xvec3(const XMFLOAT3 &vec) : vec_(XMLoadFloat3(&vec)) {}
    INLINE xvec3(FXMVECTOR vec) : vec_(vec) {}
    INLINE operator XMVECTOR() const { return vec_; }

    INLINE void SetX(f32 x) { vec_ = XMVectorSetX(vec_, x); }
    INLINE void SetY(f32 y) { vec_ = XMVectorSetY(vec_, y); }
    INLINE void SetZ(f32 z) { vec_ = XMVectorSetZ(vec_, z); }
    [[nodiscard]] INLINE scalar GetX() const { return XMVectorGetX(vec_); }
    [[nodiscard]] INLINE scalar GetY() const { return XMVectorGetY(vec_); }
    [[nodiscard]] INLINE scalar GetZ() const { return XMVectorGetZ(vec_); }

    INLINE xvec3 operator-() const { return XMVectorNegate(vec_); }
    INLINE xvec3 operator+(xvec3 v2) const { return XMVectorAdd(vec_, v2); }
    INLINE xvec3 operator-(xvec3 v2) const { return XMVectorSubtract(vec_, v2); }
    INLINE xvec3 operator*(xvec3 v2) const { return XMVectorMultiply(vec_, v2); }
    INLINE xvec3 operator/(xvec3 v2) const { return XMVectorDivide(vec_, v2); }
    INLINE xvec3 operator*(const f32 v2) const { return *this * xvec3(v2); }
    INLINE xvec3 operator/(const f32 v2) const { return *this / xvec3(v2); }

    INLINE void operator+=(const xvec3 v2) { *this = XMVectorAdd(vec_, v2); }
    INLINE void operator-=(const xvec3 v2) { *this = XMVectorSubtract(vec_, v2); }
    INLINE void operator*=(const f32 v2) { *this = *this * xvec3(v2); }
    INLINE void operator/=(const f32 v2) { *this = *this / xvec3(v2); }

    INLINE friend xvec3 operator* (scalar v1, xvec3 v2) { return xvec3(v1) * v2; }
    INLINE friend xvec3 operator/ (scalar v1, xvec3 v2) 	{ return xvec3(v1) / v2; }
private:
    XMVECTOR vec_;
};

__declspec(align(16)) class xvec4 {
public:
    INLINE xvec4() : vec_() {}
    INLINE xvec4(const f32 x, const f32 y, const f32 z, const f32 w) : vec_(XMVectorSet(x, y, z, w)) {}
    INLINE xvec4(const xvec4 &vec) : vec_(vec) {}
    INLINE xvec4(scalar scalar) : vec_(XMVectorReplicate(scalar)) {}
    INLINE xvec4(xvec3 vec) : vec_() {}
    INLINE xvec4(xvec3 xyz, f32 w) : vec_(XMVectorSetW(xyz, w)) {}
    INLINE xvec4(FXMVECTOR vec) : vec_(vec) {}
    INLINE xvec4(const XMFLOAT3 &vec) : vec_(XMLoadFloat3(&vec)) {}

    INLINE void SetX(const f32 x) { vec_ = XMVectorSetX(vec_, x); }
    INLINE void SetY(const f32 y) { vec_ = XMVectorSetY(vec_, y); }
    INLINE void SetZ(const f32 z) { vec_ = XMVectorSetZ(vec_, z); }
    INLINE void SetW(const f32 w) { vec_ = XMVectorSetW(vec_, w); }
    INLINE void SetXYZ(xvec3 xyz) { vec_ = XMVectorPermute<0,1,2,7>(xyz, vec_); }
    [[nodiscard]] INLINE scalar GetX() const { return XMVectorGetX(vec_); }
    [[nodiscard]] INLINE scalar GetY() const { return XMVectorGetY(vec_); }
    [[nodiscard]] INLINE scalar GetZ() const { return XMVectorGetZ(vec_); }
    [[nodiscard]] INLINE scalar GetW() const { return XMVectorGetW(vec_); }

    INLINE xvec4 operator-() const { return XMVectorNegate(vec_); }
    INLINE xvec4 operator+(const xvec4 v2) const { return XMVectorAdd(vec_, v2); }
    INLINE xvec4 operator-(const xvec4 v2) const { return XMVectorSubtract(vec_, v2); }
    INLINE xvec4 operator*(const xvec4 v2) const { return XMVectorMultiply(vec_, v2); }
    INLINE xvec4 operator/(const xvec4 v2) const { return XMVectorDivide(vec_, v2); }
    INLINE xvec4 operator*(const scalar s) const { return *this * xvec4(s); }
    INLINE xvec4 operator/(const scalar s) const { return *this / xvec4(s); }
    INLINE void operator+=(const xvec4 v2) { *this = XMVectorAdd(vec_, v2); }
    INLINE void operator-=(const xvec4 v2) { *this = XMVectorSubtract(vec_, v2); }
    INLINE void operator+=(const scalar s) { *this = *this + xvec4(s); }
    INLINE void operator-=(const scalar s) { *this = *this - xvec4(s); }
    INLINE void operator*=(const scalar s) { *this = *this * xvec4(s); }
    INLINE void operator/=(const scalar s) { *this = *this / xvec4(s); }
    INLINE operator DirectX::XMVECTOR() const { return vec_; }

    INLINE friend xvec4 operator* (scalar v1, xvec4 v2) { return xvec4(v1) * v2; }
    INLINE friend xvec4 operator/ (scalar v1, xvec4 v2) 	{ return xvec4(v1) / v2; }

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


using vec2 = XMFLOAT2;
using vec3 = XMFLOAT3;
using vec4 = XMFLOAT4;

}


