/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file transform.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "math/math.hpp"

namespace reveal3d::core {

class Transform {
public:
    Transform() : pos_(0), scale_(1), rot_(0)  {}
    Transform(math::xvec3 pos, math::xvec3 size, math::xvec3 rotation) : pos_(pos), scale_(size), rot_(rotation) {}
    INLINE math::mat4 World() { return math::AffineTransformation(pos_, scale_, rot_); }
    INLINE math::xvec3 Position() const { return pos_; }
    INLINE math::xvec3 Scale() { return scale_; }
    INLINE math::xvec3 Rotation() const { return rot_; }

    INLINE void SetPosition(math::xvec3 pos) { pos_ = pos; }
    INLINE void SetScale(math::xvec3 size) { scale_ = size; }
    INLINE void SetRotation(math::xvec3 rot) { rot_ = rot; }
private:
    math::xvec3 pos_;
    math::xvec3 scale_;
    math::xvec3 rot_;
};

}


