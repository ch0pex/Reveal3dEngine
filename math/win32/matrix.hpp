/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file mat4_.hpp
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

__declspec(align(16))
class mat3 {
public:
    inline mat3() {}
    inline mat3(xvec3 x, xvec3 y, xvec3 z) : mat3_({x, y, z}) {}
    inline mat3(const mat3 &mat) : mat3_({ mat.mat3_[0], mat.mat3_[1], mat.mat3_[2] }) {}
    inline mat3(const XMMATRIX &mat) : mat3_({ mat.r[0], mat.r[1], mat.r[2] }) {}
    inline operator XMMATRIX() const { return XMMATRIX(mat3_[0], mat3_[1], mat3_[2], XMVectorZero()); }

    [[nodiscard]] inline xvec3 x() const { return mat3_[0]; }
    [[nodiscard]] inline xvec3 y() const { return mat3_[1]; }
    [[nodiscard]] inline xvec3 z() const { return mat3_[2]; }
    inline void x(xvec3 x) { mat3_[0] = x; }
    inline void y(xvec3 y) { mat3_[1] = y; }
    inline void z(xvec3 z) { mat3_[2] = z; }

    inline mat3 operator*(scalar scalar) const { return mat3(scalar * mat3_[0], scalar * mat3_[1], scalar * mat3_[2]); }
    inline xvec3 operator*(xvec3 vec) const { return xvec3(XMVector3TransformNormal(vec, *this)); }
    inline mat3 operator*(const mat3 &mat) const { return mat3(*this * mat.x(), *this * mat.y(), *this * mat.z()); }

    inline mat3 make_scale(float scale) { return mat3(XMMatrixScaling(scale, scale, scale)); }
    inline mat3 make_scale(xvec3 scale) { return mat3(XMMatrixScalingFromVector(scale)); }
private:
     std::array<xvec3, 3> mat3_;
};

__declspec(align(16))
class mat4 {
public:
    inline mat4() : mat4(XMMatrixIdentity()) {}
    inline mat4(xvec3 x, xvec3 y, xvec3 z, xvec3 w) : mat4_({
                 XMVectorSetW(x, 0.0f),
                 XMVectorSetW(y, 0.0f),
                 XMVectorSetW(z, 0.0f),
                 XMVectorSetW(w, 1.0f) }) {}
    inline mat4(const float *data) { mat4_ = XMLoadFloat4x4((XMFLOAT4X4 *)data); }
    inline mat4(xvec4 x, xvec4 y, xvec4 z, xvec4 w) : mat4_(x,y,z,w) {}
    inline mat4(const mat4 &mat) : mat4_(mat.mat4_) {}
    inline mat4(const mat3 &mat) : mat4_(
            XMVectorSetW(mat.x(), 0.0f),
            XMVectorSetW(mat.y(), 0.0f),
            XMVectorSetW(mat.z(), 0.0f),
            XMVectorSet(0.0f,0.0f,0.0f,1.0f)) {}
    inline mat4(const mat3 &xyz, xvec3 w) : mat4_(
            XMVectorSetW(xyz.x(), 0),
            XMVectorSetW(xyz.y(), 0),
            XMVectorSetW(xyz.z(), 0),
            XMVectorSetW(w,1)) {}
    inline mat4(const XMMATRIX &mat) : mat4_(mat) {}
    inline void set3x3(const mat3 &xyz) {
        mat4_.r[0] = XMVectorSetW(xyz.x(), 0.0f);
        mat4_.r[1] = XMVectorSetW(xyz.y(), 0.0f);
        mat4_.r[2] = XMVectorSetW(xyz.z(), 0.0f);
    }

    [[nodiscard]] inline const mat3 &get3x3() const { return (const mat3 &) *this; }
    [[nodiscard]] inline xvec4 x() const { return mat4_.r[0]; }
    [[nodiscard]] inline xvec4 y() const { return mat4_.r[1]; }
    [[nodiscard]] inline xvec4 z() const { return mat4_.r[2]; }
    [[nodiscard]] inline xvec4 w() const { return mat4_.r[3]; }
    inline void x(xvec4 x) { mat4_.r[0] = x; }
    inline void y(xvec4 y) { mat4_.r[1] = y; }
    inline void z(xvec4 z) { mat4_.r[2] = z; }
    inline void w(xvec4 w) { mat4_.r[3] = w; }

    inline xvec3 translation() const {
        return {x().w(), y().w(), z().w() };
    }
    inline xvec3 scale() const {
        XMVECTOR scale_x = XMVector3Length(mat4_.r[0]);
        XMVECTOR scale_y = XMVector3Length(mat4_.r[1]);
        XMVECTOR scale_z = XMVector3Length(mat4_.r[2]);
        return XMVectorSet(XMVectorGetX(scale_x), XMVectorGetY(scale_y), XMVectorGetZ(scale_z), 0.0f);
    }
    inline xvec3 rotation() const {
        // Crear una matriz de rotación sin la escala
        xvec3 rotation;
//
//        f32 sy = -XMVectorGetX(mat4_.r[0]);
//        if (sy < 1.0f) {
//            if (sy > -1.0f) {
//                rotation.y(asinf(sy));
//                rotation.x(atan2f(XMVectorGetY(mat4_.r[2]), XMVectorGetZ(mat4_.r[2])));
//                rotation.z(atan2f(XMVectorGetY(mat4_.r[1]), XMVectorGetX(mat4_.r[1])));
//            } else {
//                 Not a unique solution: rotation.y == -90 degrees
//                rotation.y(-XM_PIDIV2);
//                rotation.x(-atan2f(XMVectorGetY(mat4_.r[1]), XMVectorGetY(mat4_.r[0])));
//                rotation.z(0.0f);
//            }
//        } else {
//             Not a unique solution: rotation.y == 90 degrees
//            rotation.y(XM_PIDIV2);
//            rotation.x(atan2f(XMVectorGetY(mat4_.r[1]), XMVectorGetY(mat4_.r[0])));
//            rotation.z(0.0f);
//        }

        return rotation;
    }
    inline void Decompose(xvec3& translation, xvec3& scale, xvec3& rotation) {
        translation = mat4_.r[3];

        // Extraer la escala
        XMVECTOR scale_x = XMVector3Length(mat4_.r[0]);
        XMVECTOR scale_y = XMVector3Length(mat4_.r[1]);
        XMVECTOR scale_z = XMVector3Length(mat4_.r[2]);
        scale = XMVectorSet(XMVectorGetX(scale_x), XMVectorGetY(scale_y), XMVectorGetZ(scale_z), 0.0f);

        // Crear una matriz de rotación sin la escala
        XMMATRIX rotation_matrix = mat4_;
        rotation_matrix.r[0] = XMVector3Normalize(mat4_.r[0]);
        rotation_matrix.r[1] = XMVector3Normalize(mat4_.r[1]);
        rotation_matrix.r[2] = XMVector3Normalize(mat4_.r[2]);

        // Extraer la rotación (cuaternión)
        rotation = XMQuaternionRotationMatrix(rotation_matrix);
    }

    inline operator XMMATRIX() const { return mat4_; }
    inline xvec4 operator*(xvec3 vec) const { return XMVector3Transform(vec, mat4_); }
    inline xvec4 operator*(xvec4 vec) const { return XMVector4Transform(vec, mat4_); }
    inline mat4 operator*(const mat4 &mat) const { return XMMatrixMultiply(mat4_, mat); }
    inline mat4 MakeScale(float scale) { return XMMatrixScaling(scale, scale, scale); }
    inline mat4 MakeScale(xvec3 scale) { return XMMatrixScalingFromVector(scale); }
private:
    XMMATRIX mat4_;
};

} // namespace reveal3d::math

