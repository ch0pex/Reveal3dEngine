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
    Transform(math::xvec3 pos) : pos_(pos), scale_(1), rot_(0) {}
    Transform(math::xvec3 pos, math::xvec3 size, math::xvec3 rotation) : pos_(pos), scale_(size), rot_(rotation) {}

    [[nodiscard]] INLINE math::mat4 World() const { return math::Transpose(math::AffineTransformation(pos_, scale_, rot_)); }
    [[nodiscard]] INLINE math::xvec3 Position() const { return pos_; }
    [[nodiscard]] INLINE math::xvec3 Scale() const { return scale_; }
    [[nodiscard]] INLINE math::xvec3 Rotation() const { return math::VecToDegrees(rot_); }
    [[nodiscard]] INLINE u8 IsDirty() const { return dirty_; }

//    math::xvec3& PositionRef() { return pos_; }
//    math::xvec3& ScaleRef() { return scale_; }
//    math::xvec3& RotationRef() { return rot_; }

    INLINE void SetPosition(math::xvec3 pos) { pos_ = pos; dirty_ = 3; }
    INLINE void SetScale(math::xvec3 size) { scale_ = size; dirty_ = 3; }
    INLINE void SetRotation(math::xvec3 rot) { rot_ = math::VecToRadians(rot); dirty_ = 3; }
    INLINE void UpdateDirty() { assert(dirty_ > 0); --dirty_; }
private:
    u8 dirty_ { 3 }; // If dirty should update in render buffers
    math::xvec3 pos_;
    math::xvec3 scale_;
    math::xvec3 rot_;
};

}


