/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file constants.hpp
 * @version 1.0
 * @date 11/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "math/math.hpp"

namespace reveal3d::graphics {

struct PassConstant {
    math::mat4 view;
    math::mat4 invView;
    math::mat4 proj;
    math::mat4 invProj;
    math::mat4 viewProj;
    math::mat4 invViewProj;
    math::vec2 cbPerObjectPad1;
    math::vec2 renderTargetSize;
    f32 nearZ;
    f32 farZ;
    f32 totalTime;
    f32 deltaTime;
};

struct ObjConstant {
    ObjConstant() : worldViewProj(math::Mat4Identity()) {}
    math::mat4 worldViewProj;
};

}
