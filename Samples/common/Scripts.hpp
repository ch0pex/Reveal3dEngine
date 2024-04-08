/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file Scripts.hpp
 * @version 1.0
 * @date 07/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "Engine/core/entity.hpp"
#include "Engine/math/math.hpp"

using namespace reveal3d;

// Samples scripts for Movement and rotation
class RotationScript : public core::Script {
public:
    void Update(f32 dt) override {
        math::xvec3 rot = {0.0f, 180.0f, 0.0f};
        entity_->SetRotation(entity_->Rotation() + rot * dt);
        //        entity_.HideMesh(1);
    }
    //private:

};

class MovementScript : public core::Script {
public:
    void Update(f32 dt) override {
        if (entity_->Position().GetZ() > 5.0f || entity_->Position().GetZ() < -5.0f) {
            pos_ = -pos_;
        }
        entity_->SetPosition(entity_->Position() + pos_ * dt);
    }
private:
    math::xvec3 pos_ = {0.0f, 0.0f, 1.0f};
};
