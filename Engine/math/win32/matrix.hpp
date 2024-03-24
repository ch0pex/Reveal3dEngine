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
#include <array>

namespace reveal3d::math {

__declspec(align(16)) class mat3 {
public:
    INLINE mat3() {}
    INLINE mat3(xvec3 x, xvec3 y, xvec3 z) : mat3_({x, y, z}) {}
    INLINE mat3(const mat3 &mat) : mat3_({ mat.mat3_[0], mat.mat3_[1], mat.mat3_[2] }) {}
    INLINE mat3(const XMMATRIX &mat) : mat3_({ mat.r[0], mat.r[1], mat.r[2] }) {}
    INLINE operator XMMATRIX() const { return XMMATRIX(mat3_[0], mat3_[1], mat3_[2], XMVectorZero()); }

    [[nodiscard]] INLINE xvec3 GetX() const { return mat3_[0]; }
    [[nodiscard]] INLINE xvec3 GetY() const { return mat3_[1]; }
    [[nodiscard]] INLINE xvec3 GetZ() const { return mat3_[2]; }
    INLINE void SetX(xvec3 x) { mat3_[0] = x; }
    INLINE void SetY(xvec3 y) { mat3_[1] = y; }
    INLINE void SetZ(xvec3 z) { mat3_[2] = z; }

    INLINE mat3 operator*(scalar scalar) const { return mat3(scalar * mat3_[0], scalar * mat3_[1], scalar * mat3_[2]); }
    INLINE xvec3 operator*(xvec3 vec) const { return xvec3(XMVector3TransformNormal(vec, *this)); }
    INLINE mat3 operator*(const mat3 &mat) const { return mat3(*this * mat.GetX(), *this * mat.GetY(), *this * mat.GetZ()); }

    INLINE mat3 MakeScale(float scale) { return mat3(XMMatrixScaling(scale, scale, scale)); }
    INLINE mat3 MakeScale(xvec3 scale) { return mat3(XMMatrixScalingFromVector(scale)); }
private:
     std::array<xvec3, 3> mat3_;
};

__declspec(align(16)) class mat4 {
public:
    INLINE mat4() {}
    INLINE mat4(xvec3 x, xvec3 y, xvec3 z, xvec3 w) : mat4_({
                 XMVectorSetW(x, 0.0f),
                 XMVectorSetW(y, 0.0f),
                 XMVectorSetW(z, 0.0f),
                 XMVectorSetW(w, 1.0f) }) {}
    INLINE mat4(const float *data) { mat4_ = XMLoadFloat4x4((XMFLOAT4X4 *)data); }
    INLINE mat4(xvec4 x, xvec4 y, xvec4 z, xvec4 w) : mat4_(x,y,z,w) {}
    INLINE mat4(const mat4 &mat) : mat4_(mat.mat4_) {}
    INLINE mat4(const mat3 &mat) : mat4_(
            XMVectorSetW(mat.GetX(), 0.0f),
            XMVectorSetW(mat.GetY(), 0.0f),
            XMVectorSetW(mat.GetZ(), 0.0f),
            XMVectorSet(0.0f,0.0f,0.0f,1.0f)) {}
    INLINE mat4(const mat3 &xyz, xvec3 w) : mat4_(
            XMVectorSetW(xyz.GetX(), 0),
            XMVectorSetW(xyz.GetY(), 0),
            XMVectorSetW(xyz.GetZ(), 0),
            XMVectorSetW(w,1)) {}
    INLINE mat4(const XMMATRIX &mat) : mat4_(mat) {}
    INLINE void Set3x3(const mat3 &xyz) {
        mat4_.r[0] = XMVectorSetW(xyz.GetX(), 0.0f);
        mat4_.r[1] = XMVectorSetW(xyz.GetY(), 0.0f);
        mat4_.r[2] = XMVectorSetW(xyz.GetZ(), 0.0f);
    }

    [[nodiscard]] INLINE const mat3 &Get3x3() const { return (const mat3 &) *this; }
    [[nodiscard]] INLINE xvec4 GetX() const { return mat4_.r[0]; }
    [[nodiscard]] INLINE xvec4 GetY() const { return mat4_.r[1]; }
    [[nodiscard]] INLINE xvec4 GetZ() const { return mat4_.r[2]; }
    [[nodiscard]] INLINE xvec4 GetW() const { return mat4_.r[3]; }
    INLINE void SetX(xvec4 x) { mat4_.r[0] = x; }
    INLINE void SetY(xvec4 y) { mat4_.r[1] = y; }
    INLINE void SetZ(xvec4 z) { mat4_.r[2] = z; }
    INLINE void SetW(xvec4 w) { mat4_.r[3] = w; }

    INLINE operator XMMATRIX() const { return mat4_; }
    INLINE xvec4 operator*(xvec3 vec) const { return XMVector3Transform(vec, mat4_); }
    INLINE xvec4 operator*(xvec4 vec) const { return XMVector4Transform(vec, mat4_); }
    INLINE mat4 operator*(const mat4 &mat) const { return XMMatrixMultiply(mat4_, mat); }
    INLINE mat4 MakeScale(float scale) { return XMMatrixScaling(scale, scale, scale); }
    INLINE mat4 MakeScale(xvec3 scale) { return XMMatrixScalingFromVector(scale); }
private:
    XMMATRIX mat4_;
};

} // namespace reveal3d::math

