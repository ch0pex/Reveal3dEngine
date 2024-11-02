/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file scalar.hpp
 * @version 1.0
 * @date 02/03/2024
 * @brief scalar class for math
 *
 * This scalar class is mean to wrap XMVector class in windows systems
 * in order to operate faster
 */

#pragma once

#include "common/common.hpp"

namespace reveal3d::math {

class scalar {
public:
    scalar() = default;
    scalar(const scalar& s) : vec_(s) {}
    scalar(f32 f) : vec_(XMVectorReplicate(f)) {}
    scalar(FXMVECTOR vec) : vec_(vec) {}
    operator XMVECTOR() const { return vec_; }
    operator f32() const { return XMVectorGetX(vec_); }

private:
    XMVECTOR vec_;
};

inline scalar operator-(scalar s) { return scalar(XMVectorNegate(s)); }
inline scalar operator+(scalar s1, scalar s2) { return scalar(XMVectorAdd(s1, s2)); }
inline scalar operator-(scalar s1, scalar s2) { return scalar(XMVectorSubtract(s1, s2)); }
inline scalar operator*(scalar s1, scalar s2) { return scalar(XMVectorMultiply(s1, s2)); }
inline scalar operator/(scalar s1, scalar s2) { return scalar(XMVectorDivide(s1, s2)); }
inline scalar operator+(scalar s, f32 f) { return s + scalar(f); }
inline scalar operator-(scalar s, f32 f) { return s - scalar(f); }
inline scalar operator*(scalar s, f32 f) { return s * scalar(f); }
inline scalar operator/(scalar s, f32 f) { return s / scalar(f); }
inline scalar operator+(f32 f, scalar s) { return scalar(f) + s; }
inline scalar operator-(f32 f, scalar s) { return scalar(f) - s; }
inline scalar operator*(f32 f, scalar s) { return scalar(f) * s; }
inline scalar operator/(f32 f, scalar s) { return scalar(f) / s; }
inline bool operator<(scalar s, f32 f) { return static_cast<f32>(s) < f; }
inline bool operator<=(scalar s, f32 f) { return static_cast<f32>(s) <= f; }
inline bool operator>(scalar s, f32 f) { return static_cast<f32>(s) > f; }
inline bool operator>=(scalar s, f32 f) { return static_cast<f32>(s) >= f; }
inline bool operator==(scalar s, f32 f) { return static_cast<f32>(s) == f; }
inline bool operator<(f32 f, scalar s) { return f < static_cast<f32>(s); }
inline bool operator<=(f32 f, scalar s) { return f <= static_cast<f32>(s); }
inline bool operator>(f32 f, scalar s) { return f > static_cast<f32>(s); }
inline bool operator>=(f32 f, scalar s) { return f >= static_cast<f32>(s); }
inline bool operator==(f32 f, scalar s) { return f == static_cast<f32>(s); }
} // namespace reveal3d::math
