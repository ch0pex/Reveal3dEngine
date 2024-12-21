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

#include "vector.hpp"
#include "scalar.hpp"
#include "matrix.hpp"

#include <concepts>

namespace reveal3d::math {

// Concept for T, it must be XMVector wrapper class (scalar, xvec3, xvec4)
template<typename T>
concept is_vector = requires(T t) {
    { t } -> std::convertible_to<scalar>;
    { t } -> std::convertible_to<xvec3>;
    { t } -> std::convertible_to<xvec4>;
};

template<is_vector T> T sqrt(T s) { return XMVectorSqrt(s); }
template<is_vector T> T recip(T s) { return T(XMVectorReciprocal(s)); }
template<is_vector T> T recip_sqrt(T s) { return T(XMVectorReciprocalSqrt(s)); }
template<is_vector T> T floor(T s) { return T(XMVectorFloor(s)); }
template<is_vector T> T ceiling(T s) { return T(XMVectorCeiling(s)); }
template<is_vector T> T round(T s) { return T(XMVectorRound(s)); }
template<is_vector T> T abs(T s) { return T(XMVectorAbs(s)); }
template<is_vector T> T exp(T s) { return T(XMVectorExp(s)); }
template<is_vector T> T pow(T b, T e) { return T(XMVectorPow(b, e)); }
template<is_vector T> T log(T s) { return T(XMVectorLog(s)); }
template<is_vector T> T sin(T s) { return T(XMVectorSin(s)); }
template<is_vector T> T cos(T s) { return T(XMVectorCos(s)); }
template<is_vector T> T tan(T s) { return T(XMVectorTan(s)); }
template<is_vector T> T a_sin(T s) { return T(XMVectorASin(s)); }
template<is_vector T> T a_cos(T s) { return T(XMVectorACos(s)); }
template<is_vector T> T a_tan(T s) { return T(XMVectorATan(s)); }
template<is_vector T> T a_tan2(T y, T x) { return T(XMVectorATan2(y, x)); }
template<is_vector T> T lerp(T a, T b, T t) { return T(XMVectorLerpV(a, b, t)); }
template<is_vector T> T lerp(T a, T b, f32 t) { return T(XMVectorLerp(a, b, t)); }
template<is_vector T> T max_(T a, T b) { return T(XMVectorMax(a, b)); }
template<is_vector T> T min_(T a, T b) { return T(XMVectorMin(a, b)); }
template<is_vector T> T clamp(T v, T a, T b) { return min_(max_(v, a), b); }
template<is_vector T> BoolVector operator<(T lhs, T rhs) { return XMVectorLess(lhs, rhs); }
template<is_vector T> BoolVector operator<=(T lhs, T rhs) { return XMVectorLessOrEqual(lhs, rhs); }
template<is_vector T> BoolVector operator>(T lhs, T rhs) { return XMVectorGreater(lhs, rhs); }
template<is_vector T> BoolVector operator>=(T lhs, T rhs) { return XMVectorGreaterOrEqual(lhs, rhs); }
template<is_vector T> BoolVector operator==(T lhs, T rhs) { return XMVectorEqual(lhs, rhs); }
template<is_vector T> T select(T lhs, T rhs, BoolVector mask) { return T(XMVectorSelect(lhs, rhs, mask)); }
inline f32 sqrt(f32 s) { return sqrt(scalar(s)); }
inline f32 recip(f32 s) { return recip(scalar(s)); }
inline f32 recipSqrt(f32 s) { return recip_sqrt(scalar(s)); }
inline f32 floor(f32 s) { return floor(scalar(s)); }
inline f32 ceiling(f32 s) { return ceiling(scalar(s)); }
inline f32 round(f32 s) { return round(scalar(s)); }
inline f32 abs(f32 s) { return s < 0.0f ? -s : s; }
inline f32 exp(f32 s) { return exp(scalar(s)); }
inline f32 pow(f32 b, f32 e) { return pow(scalar(b), scalar(e)); }
inline f32 log(f32 s) { return log(scalar(s)); }
inline f32 sin(f32 s) { return sin(scalar(s)); }
inline f32 cos(f32 s) { return cos(scalar(s)); }
inline f32 tan(f32 s) { return tan(scalar(s)); }
inline f32 a_sin(f32 s) { return a_sin(scalar(s)); }
inline f32 a_cos(f32 s) { return a_cos(scalar(s)); }
inline f32 a_tan(f32 s) { return a_tan(scalar(s)); }
inline f32 a_ta2(f32 y, f32 x) { return a_tan2(scalar(y), scalar(x)); }
inline f32 lerp(f32 a, f32 b, f32 t) { return a + (b - a) * t; }
inline f32 max_(f32 a, f32 b) { return a > b ? a : b; }
inline f32 min_(f32 a, f32 b) { return a < b ? a : b; }
inline f32 clamp(f32 v, f32 a, f32 b) { return min_(max_(v, a), b); }
inline f32 radians(f32 s) { return XMConvertToRadians(s); }

inline scalar length(xvec3 v) { return scalar(XMVector3Length(v)); }
inline scalar lengthSquare(xvec3 v) { return scalar(XMVector3LengthSq(v)); }
inline scalar lengthRecip(xvec3 v) { return scalar(XMVector3ReciprocalLength(v)); }
inline scalar dot(xvec3 v1, xvec3 v2) { return scalar(XMVector3Dot(v1, v2)); }
inline scalar dot(xvec4 v1, xvec4 v2) { return scalar(XMVector4Dot(v1, v2)); }
inline xvec3 cross(xvec3 v1, xvec3 v2) { return xvec3(XMVector3Cross(v1, v2)); }
inline xvec3 normalize(xvec3 v) { return xvec3(XMVector3Normalize(v)); }
inline xvec4 normalize(xvec4 v) { return xvec4(XMVector4Normalize(v)); }
inline xvec3 vec_to_radians(xvec3 v) {
    return {XMConvertToRadians(v.x()), XMConvertToRadians(v.y()), XMConvertToRadians(v.z())};
}
inline xvec3 vec_to_degrees(xvec3 v) {
    return {XMConvertToDegrees(v.x()), XMConvertToDegrees(v.y()), XMConvertToDegrees(v.z())};
}
inline mat3 transpose(const mat3& mat) { return mat3(XMMatrixTranspose(mat)); }
inline mat4 transpose(const mat4& mat) { return mat4(XMMatrixTranspose(mat)); }
inline mat4 look_at(const xvec3 position, const xvec3 focus_point, const xvec3 up_dir) {
    return XMMatrixLookAtLH(position, focus_point, up_dir);
}
inline mat4 perspective_fov(const f32 fov, const f32 aspect_ratio, const f32 near_plane, const f32 far_plane) {
    return XMMatrixPerspectiveFovLH(radians(fov), aspect_ratio, near_plane, far_plane);
}

// inline mat4 AffineTransformation(const xvec3 position, const xvec3 scale, const xvec3 rotation ) { return
// XMMatrixAffineTransformation(scale, {0.0f, 0.0f, 0.0f}, XMQuaternionRotationRollPitchYawFromVector(rotation),
// position);}

inline mat4 affine_transformation(const xvec3 position, const xvec3 scale, const xvec3 rotation) {
    return XMMatrixScalingFromVector(scale) * XMMatrixRotationRollPitchYawFromVector(rotation) *
           XMMatrixTranslationFromVector(position);
}
inline mat4 inverse(const mat4& mat) { return XMMatrixInverse(nullptr, mat); }
inline mat4 mat4_identity() { return XMMatrixIdentity(); }

inline mat3 inverse_transpose(const mat3& mat) {
    const xvec3 x = mat.x();
    const xvec3 y = mat.y();
    const xvec3 z = mat.z();

    const xvec3 inv0 = cross(y, z);
    const xvec3 inv1 = cross(z, x);
    const xvec3 inv2 = cross(x, y);
    const scalar r_det = recip(dot(z, inv2));

    // Return the adjoin / determinant
    return mat3(inv0, inv1, inv2) * r_det;
}

inline mat4 ortho_invert(const mat4& xform) {
    const mat3 basis = transpose(xform.get3x3());
    const xvec3 translate = basis * -xvec3(xform.w());
    return {basis, translate};
}

} // namespace reveal3d::math
