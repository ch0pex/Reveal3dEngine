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

namespace reveal3d::math {

//Concept for T, it must be XMVector wrapper class (scalar, xvec3, xvec4)

template<typename T> INLINE T Sqrt(T s) { return T(XMVectorSqrt(s)); }
template<typename T> INLINE T Recip(T s) { return T(XMVectorReciprocal(s)); }
template<typename T> INLINE T RecipSqrt(T s) { return T(XMVectorReciprocalSqrt(s)); }
template<typename T> INLINE T Floor(T s) { return T(XMVectorFloor(s)); }
template<typename T> INLINE T Ceiling(T s) { return T(XMVectorCeiling(s)); }
template<typename T> INLINE T Round(T s) { return T(XMVectorRound(s)); }
template<typename T> INLINE T Abs(T s) { return T(XMVectorAbs(s)); }
template<typename T> INLINE T Exp(T s) { return T(XMVectorExp(s)); }
template<typename T> INLINE T Pow(T b, T e) { return T(XMVectorPow(b, e)); }
template<typename T> INLINE T Log(T s) { return T(XMVectorLog(s)); }
template<typename T> INLINE T Sin(T s) { return T(XMVectorSin(s)); }
template<typename T> INLINE T Cos(T s) { return T(XMVectorCos(s)); }
template<typename T> INLINE T Tan(T s) { return T(XMVectorTan(s)); }
template<typename T> INLINE T ASin(T s) { return T(XMVectorASin(s)); }
template<typename T> INLINE T ACos(T s) { return T(XMVectorACos(s)); }
template<typename T> INLINE T ATan(T s) { return T(XMVectorATan(s)); }
template<typename T> INLINE T ATan2(T y, T x) { return T(XMVectorATan2(y, x)); }
template<typename T> INLINE T Lerp(T a, T b, T t) { return T(XMVectorLerpV(a, b, t)); }
template<typename T> INLINE T Lerp(T a, T b, f32 t ) { return T(XMVectorLerp(a, b, t)); }
template<typename T> INLINE T Max(T a, T b) { return T(XMVectorMax(a, b)); }
template<typename T> INLINE T Min(T a, T b) { return T(XMVectorMin(a, b)); }
template<typename T> INLINE T Clamp(T v, T a, T b) { return Min(Max(v, a), b); }
template<typename T> INLINE BoolVector operator<  (T lhs, T rhs) { return XMVectorLess(lhs, rhs); }
template<typename T> INLINE BoolVector operator<= (T lhs, T rhs) { return XMVectorLessOrEqual(lhs, rhs); }
template<typename T> INLINE BoolVector operator>  (T lhs, T rhs) { return XMVectorGreater(lhs, rhs); }
template<typename T> INLINE BoolVector operator>= (T lhs, T rhs) { return XMVectorGreaterOrEqual(lhs, rhs); }
template<typename T> INLINE BoolVector operator== (T lhs, T rhs) { return XMVectorEqual(lhs, rhs); }
template<typename T> INLINE T Select(T lhs, T rhs, BoolVector mask) { return T(XMVectorSelect(lhs, rhs, mask)); }

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
INLINE f32 Radians(f32 s) { return XMConvertToRadians(s); }

INLINE scalar Length(xvec3 v) { return scalar(XMVector3Length(v)); }
INLINE scalar LengthSquare(xvec3 v) { return scalar(XMVector3LengthSq(v)); }
INLINE scalar LengthRecip(xvec3 v) { return scalar(XMVector3ReciprocalLength(v)); }
INLINE scalar Dot(xvec3 v1, xvec3 v2) { return scalar(XMVector3Dot(v1, v2)); }
INLINE scalar Dot(xvec4 v1, xvec4 v2) { return scalar(XMVector4Dot(v1, v2)); }
INLINE xvec3 Cross(xvec3 v1, xvec3 v2) { return xvec3(XMVector3Cross(v1, v2)); }
INLINE xvec3 Normalize(xvec3 v) { return xvec3(XMVector3Normalize(v)); }
INLINE xvec4 Normalize(xvec4 v) { return xvec4(XMVector4Normalize(v)); }
INLINE mat3 Transpose(const mat3 & mat) { return mat3(XMMatrixTranspose(mat)); }
INLINE mat4 LookAt(xvec3 position, xvec3 focusPoint, xvec3 upDir) { return XMMatrixLookAtLH(position, focusPoint, upDir); }
INLINE mat4 PerspectiveFov(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane) {return XMMatrixPerspectiveFovLH(Radians(fov), aspectRatio, nearPlane, farPlane);}

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