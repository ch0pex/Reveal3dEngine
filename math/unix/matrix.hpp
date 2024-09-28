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

namespace reveal3d::math {

template<u32 T>
class matrix {
public:
    matrix() {}
//    matrix(xvec3 x, xvec3 y, xvec3 z, xvec3 w) {
//    }
    matrix(const f32 *data) {}
    matrix(const glm::mat4 &mat) : mat_(mat) {}
//    matrix(xvec3 x, xvec3 y, xvec3 z) : mat_(x,y,z) {}
//    matrix(xvec4 x, xvec4 y, xvec4 z, xvec4 w) : mat_(x,y,z,w) {}
    matrix(const matrix &mat) : mat_(mat.mat_) {}
//    matrix(const matrix &xyz, matrix w) : mat4_(
//            XMVectorSetW(xyz.GetX(), 0),
//            XMVectorSetW(xyz.GetY(), 0),
//            XMVectorSetW(xyz.GetZ(), 0),
//            XMVectorSetW(w,1)) {}
//    matrix(const matrix &mat) : mat_(mat) {}
//    void Set3x3(const matrix &xyz) {
//        mat4_.r[0] = XMVectorSetW(xyz.GetX(), 0.0f);
//        mat4_.r[1] = XMVectorSetW(xyz.GetY(), 0.0f);
//        mat4_.r[2] = XMVectorSetW(xyz.GetZ(), 0.0f);
//    }

    [[nodiscard]] const matrix &Get3x3() const { return (const matrix &) *this; }
    [[nodiscard]] vector<T> GetX() const { return mat_[0]; }
    [[nodiscard]] vector<T> GetY() const { return mat_[1]; }
    [[nodiscard]] vector<T> GetZ() const { return mat_[2]; }
    [[nodiscard]] vector<T> GetW() const { return mat_[3]; }
    void SetX(vector<T> x) { mat_.r[0] = x; }
    void SetY(vector<T> y) { mat_.r[1] = y; }
    void SetZ(vector<T> z) { mat_.r[2] = z; }
    void SetW(vector<T> w) { mat_.r[3] = w; }


    operator glm::mat<T,T, f32>() const { return mat_; }
//    xvec4 operator*(xvec3 vec) const { return vec * mat_; }
//    xvec4 operator*(xvec4 vec) const { return vec * mat_; }
    matrix operator*(const matrix &mat) const { return mat.mat_ * mat_; }
    matrix MakeScale(scalar scale) {
        return scale * mat_; }
//    matrix MakeScale(xvec3 scale) { return XMMatrixScalingFromVector(scale); }
private:
    glm::mat<T,T,f32> mat_;
};


using mat3 = matrix<3>;
using mat4 = matrix<4>;

} // namespace reveal3d::math

