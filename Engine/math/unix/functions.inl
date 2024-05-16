/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file functions.hpp
 * @version 1.0
 * @date 01/03/2024
 * @brief Math functiosn
 *
 * Math inline template functions for vector, scalar and matrix
 */

#include <concepts>

namespace reveal3d::math {
//Concept for T, it must be XMVector wrapper class (scalar, xvec3, xvec4)
template<typename T>
concept vectorizable = requires(T t) {
    {t} -> std::convertible_to<scalar>;
    {t} -> std::convertible_to<xvec3>;
    {t} -> std::convertible_to<xvec4>;
};

template<vectorizable T> T Sqrt(T s) {}
template<vectorizable T> T Recip(T s) {}
template<vectorizable T> T RecipSqrt(T s) {}
template<vectorizable T> T Floor(T s) {}
template<vectorizable T> T Ceiling(T s) {}
template<vectorizable T> T Round(T s) {}
template<vectorizable T> T Abs(T s) {}
template<vectorizable T> T Exp(T s) {}
template<vectorizable T> T Pow(T b, T e) {}
template<vectorizable T> T Log(T s) {}
template<vectorizable T> T Sin(T s) {}
template<vectorizable T> T Cos(T s) {}
template<vectorizable T> T Tan(T s) {}
template<vectorizable T> T ASin(T s) {}
template<vectorizable T> T ACos(T s) {}
template<vectorizable T> T ATan(T s) {}
template<vectorizable T> T ATan2(T y, T x) {}
template<vectorizable T> T Lerp(T a, T b, T t) {}
template<vectorizable T> T Lerp(T a, T b, f32 t ) {}
template<vectorizable T> T Max(T a, T b) {}
template<vectorizable T> T Min(T a, T b) {}
template<vectorizable T> T Clamp(T v, T a, T b) {}
template<vectorizable T> BoolVector operator<  (T lhs, T rhs) {}
template<vectorizable T> BoolVector operator<= (T lhs, T rhs) {}
template<vectorizable T> BoolVector operator>  (T lhs, T rhs) {}
template<vectorizable T> BoolVector operator>= (T lhs, T rhs) {}
template<vectorizable T> BoolVector operator== (T lhs, T rhs) {}
template<vectorizable T> T Select(T lhs, T rhs, BoolVector mask) {}

INLINE f32 Sqrt(f32 s) { return Sqrt(scalar(s)); }
INLINE f32 Recip(f32 s) { return Recip(scalar(s)); }
INLINE f32 RecipSqrt(f32 s) { return RecipSqrt(scalar(s)); }
INLINE f32 Floor(f32 s) { return Floor(scalar(s)); }
INLINE f32 Ceiling( f32 s) { return Ceiling(scalar(s)); }
INLINE f32 Round(f32 s) { return Round(scalar(s)); }
INLINE f32 Abs(f32 s) { return s < 0.0f ? -s : s; }
INLINE f32 Exp(f32 s) { return Exp(scalar(s)); }
INLINE f32 Pow(f32 b, f32 e) { return Pow(scalar(b), scalar(e)); }
INLINE f32 Log(f32 s) { return Log(scalar(s)); }
INLINE f32 Sin(f32 s) { return Sin(scalar(s)); }
INLINE f32 Cos(f32 s) { return Cos(scalar(s)); }
INLINE f32 Tan(f32 s) { return Tan(scalar(s)); }
INLINE f32 ASin(f32 s) { return ASin(scalar(s)); }
INLINE f32 ACos(f32 s) { return ACos(scalar(s)); }
INLINE f32 ATan(f32 s) { return ATan(scalar(s)); }
INLINE f32 ATan2(f32 y, f32 x) { return ATan2(scalar(y), scalar(x)); }
INLINE f32 Lerp(f32 a, f32 b, f32 t) { return a + (b - a) * t; }
INLINE f32 Max(f32 a, f32 b) { return a > b ? a : b; }
INLINE f32 Min(f32 a, f32 b) { return a < b ? a : b; }
INLINE f32 Clamp(f32 v, f32 a, f32 b) { return Min(Max(v, a), b); }
INLINE f32 Radians(f32 s) {}

INLINE scalar Length(xvec3 v) {}
INLINE scalar LengthSquare(xvec3 v) {}
INLINE scalar LengthRecip(xvec3 v) {}
INLINE scalar Dot(xvec3 v1, xvec3 v2) {}
INLINE scalar Dot(xvec4 v1, xvec4 v2) {}
INLINE xvec3 Cross(xvec3 v1, xvec3 v2) {}
INLINE xvec3 Normalize(xvec3 v) {}
INLINE xvec4 Normalize(xvec4 v) {}
INLINE xvec3 VecToRadians(xvec3 v) {}
INLINE xvec3 VecToDegrees(xvec3 v) {}
INLINE mat3 Transpose(const mat3 & mat) {}
INLINE mat4 Transpose(const mat4 & mat) {}
INLINE mat4 LookAt(xvec3 position, xvec3 focusPoint, xvec3 upDir) {}
INLINE mat4 PerspectiveFov(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane) {}
//INLINE mat4 AffineTransformation(const xvec3 position, const xvec3 scale, const xvec3 rotation ) { return XMMatrixAffineTransformation(scale, {0.0f, 0.0f, 0.0f}, XMQuaternionRotationRollPitchYawFromVector(rotation), position);}
INLINE mat4 AffineTransformation(const xvec3 position, const xvec3 scale, const xvec3 rotation ) {}
INLINE mat4 Mat4Identity() {}

INLINE mat3 InverseTranspose(const mat3 & mat)
{
    const xvec3 x = mat.GetX();
    const xvec3 y = mat.GetY();
    const xvec3 z = mat.GetZ();

    const xvec3 inv0 = Cross(y, z);
    const xvec3 inv1 = Cross(z, x);
    const xvec3 inv2 = Cross(x, y);
    const scalar rDet = Recip(Dot(z, inv2));

    // Return the adjoint / determinant
    return mat3(inv0, inv1, inv2) * rDet;
}

INLINE mat4 OrthoInvert(const mat4 & xform)
{
    mat3 basis = Transpose(xform.Get3x3());
    xvec3 translate = basis * -xvec3(xform.GetW());
    return mat4(basis, translate);
}
}
