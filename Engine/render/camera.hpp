/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file camera.hpp
 * @version 1.0
 * @date 01/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "math/math.hpp"
#include "window/window_info.hpp"

namespace reveal3d::render {

class Camera {
public:
    explicit Camera(const window::Resolution &res);

    void Update(const Timer& timer);
    void OnResize();

    [[nodiscard]] INLINE math::mat4 GetProjectionMatrix() const { return projectionMatrix_; }
    [[nodiscard]] INLINE math::mat4 GetViewMatrix() const { return viewMatrix_; }

private:
    math::xvec3 position_;
    math::mat4 projectionMatrix_;
    math::mat4 viewMatrix_;
    math::mat4 viewProjectionMatrix_;
    //TODO: Frustum

};

}

