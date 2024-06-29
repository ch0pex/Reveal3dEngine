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

#include "Engine/core/scene.hpp"
#include "Engine/math/math.hpp"

using namespace reveal3d;

// Samples scripts for Movement and rotation
class HumanScript : public core::Script {
public:
    using Script::Script;

    void Begin(core::Entity &entity) override {
        startPos = entity.Transform().Position();
    }

    void Update(core::Entity &entity, f32 dt) override {
        const math::xvec3 rot = {0.0f, 0.0f, 90.0f};
        entity.Transform().SetRotation(entity.Transform().Rotation() + rot * dt);
        f32 posX = startPos.GetX();
        if (entity.Transform().Position().GetX() >= (posX + 20.0f) || entity.Transform().Position().GetX() < f32(startPos.GetX())) {
            dir_ = -dir_;
        }
//        entity.SetPosition(entity.Position() + dir_ * dt);
    }
    private:
        math::xvec3 dir_ = {1.0f, 0.0f, 0.0f};
        math::xvec3 startPos = {};
};
