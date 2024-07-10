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

template<u32 dim, u32 value>
concept Dimension = value >= dim;

template<u32 T>
requires Dimension<2, T>
class vector {
public:
    vector() : vec_() {}
    template<u32 value = T> requires Dimension<3, value> vector(const f32 x, const f32 y, const f32 z) : vec_{x,y,z} {}
    template<u32 value = T> requires Dimension<4, value> vector(const f32 x, const f32 y, const f32 z, const f32 w) : vec_{x,y,z,w} {}
    vector(const glm::vec<T, f32> &vec) : vec_(vec) {}
    vector(const scalar scalar) : vec_(glm::vec3(scalar)) {}
//    xvec3(const glm::vec3 &vec) : vec_(vec) {}
    template<u32 value = T> requires Dimension<3, value> vector(glm::vec3 vec) : vec_(vec) {}
    template<u32 value = T> requires Dimension<4, value> vector(glm::vec4 vec) : vec_(vec) {}
    template<u32 value = T> requires Dimension<3, value> operator glm::vec3() const { return vec_; }
    template<u32 value = T> requires Dimension<4, value> operator glm::vec4() const { return vec_; }

    void SetX(f32 x) { vec_.x = x; }
    void SetY(f32 y) { vec_.y = y; }
    template<u32 value = T> requires Dimension<3, value> void SetZ(f32 z) { vec_.z = z; }
    template<u32 value = T> requires Dimension<4, value>void SetW(f32 w) { vec_.w = w; }

    [[nodiscard]] scalar GetX() const { return vec_.x; }
    [[nodiscard]] scalar GetY() const { return vec_.y; }
    template<u32 value = T> requires Dimension<3, value> [[nodiscard]] scalar GetZ() const { return vec_.z; }
    template<u32 value = T> requires Dimension<4, value> [[nodiscard]] scalar GetW() const { return vec_.z; }

    vector operator-() const { return vec_; }
    vector operator+(vector v2) const { return vec_ + glm::vec<T, f32>(v2); }
    vector operator-(vector v2) const { return vec_ - glm::vec<T, f32>(v2); }
    vector operator*(vector v2) const { return vec_ * glm::vec<T, f32>(v2); }
    vector operator/(vector v2) const { return vec_ / glm::vec<T, f32>(v2); }
    vector operator*(const f32 v2) const { return *this * vector(v2); }
    vector operator/(const f32 v2) const { return *this / vector(v2); }

    void operator+=(const vector v2) { *this = *this + v2; }
    void operator-=(const vector v2) { *this = *this - v2; }
    void operator*=(const f32 v2) { *this = *this * v2; }
    void operator/=(const f32 v2) { *this = *this / v2; }

    friend vector operator* (scalar v1, vector v2) { return vector(v1) * v2; }
    friend vector operator/ (scalar v1, vector v2) 	{ return vector(v1) / v2; }
private:
    glm::vec<T, f32> vec_;
};



class BoolVector
{
    //TODO
};


using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

using xvec3 = vector<3>;
using xvec4 = vector<4>;

}


