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
    INLINE scalar() = default;
    INLINE scalar(const scalar & s) : vec_(s) {}
    INLINE scalar(f32 f) : vec_ (XMVectorReplicate(f)) {}
    INLINE explicit scalar( FXMVECTOR vec ) : vec_(vec) {}
    INLINE operator XMVECTOR() const { return vec_; }
    INLINE operator f32() const { return XMVectorGetX(vec_); }

private:
    XMVECTOR vec_;
};

INLINE scalar operator- (scalar s) { return scalar(XMVectorNegate(s)); }
INLINE scalar operator+ (scalar s1, scalar s2) { return scalar(XMVectorAdd(s1, s2)); }
INLINE scalar operator- (scalar s1, scalar s2) { return scalar(XMVectorSubtract(s1, s2)); }
INLINE scalar operator* (scalar s1, scalar s2) { return scalar(XMVectorMultiply(s1, s2)); }
INLINE scalar operator/ (scalar s1, scalar s2) { return scalar(XMVectorDivide(s1, s2)); }
INLINE scalar operator+ (scalar s, f32 f) { return s + scalar(f); }
INLINE scalar operator- (scalar s, f32 f) { return s - scalar(f); }
INLINE scalar operator* (scalar s, f32 f) { return s * scalar(f); }
INLINE scalar operator/ (scalar s, f32 f) { return s / scalar(f); }
INLINE scalar operator+ (f32 f, scalar s) { return scalar(f) + s; }
INLINE scalar operator- (f32 f, scalar s) { return scalar(f) - s; }
INLINE scalar operator* (f32 f, scalar s) { return scalar(f) * s; }
INLINE scalar operator/ (f32 f, scalar s) { return scalar(f) / s; }
INLINE bool operator<  (scalar s, f32 f) { return (f32)s <  f; }
INLINE bool operator<= (scalar s, f32 f) { return (f32)s <= f; }
INLINE bool operator>  (scalar s, f32 f) { return (f32)s >  f; }
INLINE bool operator>= (scalar s, f32 f) { return (f32)s >= f; }
INLINE bool operator== (scalar s, f32 f) { return (f32)s == f; }
INLINE bool operator<  (f32 f, scalar s) { return f <  (f32)s; }
INLINE bool operator<= (f32 f, scalar s) { return f <= (f32)s; }
INLINE bool operator>  (f32 f, scalar s) { return f >  (f32)s; }
INLINE bool operator>= (f32 f, scalar s) { return f >= (f32)s; }
INLINE bool operator== (f32 f, scalar s) { return f == (f32)s; }

}
