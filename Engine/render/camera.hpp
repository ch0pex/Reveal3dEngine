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
#include "common/timer.hpp"

namespace reveal3d::render {

class Camera {
public:
    explicit Camera(const window::Resolution &res);

    void Update(const Timer& timer);
    void Resize(const window::Resolution &res);
    // TODO: Frustum

    [[nodiscard]] INLINE math::mat4 GetProjectionMatrix() const { return projectionMatrix_; }
    [[nodiscard]] INLINE math::mat4 GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
    [[nodiscard]] INLINE math::mat4 GetViewMatrix() const { return viewMatrix_; }

    //Input handlers
    void Move(input::action dir, input::type inputType);
    void LookAt(input::action dir, math::xvec3 mousePos);
    void SetLooking(input::action act, input::type inputType);

private:
    void UpdatePos(math::scalar dt);

    math::xvec3 position_;
    math::xvec3 lookDir_;
    math::xvec3 upDirection_;
    math::mat4 projectionMatrix_;
    math::mat4 viewMatrix_;
    math::mat4 viewProjectionMatrix_;

    math::scalar moveSpeed_;
    input::System<Camera> inputSys_;

    enum dir {
        fwd,
        bckwd,
        up,
        down,
        left,
        right,

        count
    };

    bool isMoving[dir::count] { false }; // Is moving to certain direction
    bool isLooking { false }; // Middle mouse button is pressed for looking at
};

}

