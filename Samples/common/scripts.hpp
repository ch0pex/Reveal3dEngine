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
class HumanScript : public core::Script {
public:
    using Script::Script;

    void Begin(core::Entity &entity) override {
        startXPos_ = entity.Position().GetZ();
    }

    void Update(core::Entity &entity, f32 dt) override {
        math::xvec3 rot = {0.0f, 0.0f, 90.0f};
        entity.SetRotation(entity.Rotation() + rot * dt);
        if (entity.Position().GetX() >= 20.0f || entity.Position().GetX() < -20.0f) {
            dir_ = -dir_;
        }
        entity.SetPosition(entity.Position() + dir_ * dt);
    }
    private:
        math::xvec3 dir_ = {1.0f, 0.0f, 0.0f};
        f32 startXPos_;
};
