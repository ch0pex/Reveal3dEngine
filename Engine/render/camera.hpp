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

    /********* Input handling ************/
    void Move(input::action dir, input::type value);
    void SetLooking(input::action action, input::type value);
    void SetNewMousePos(input::action action, math::vec2 mousePos);

private:
    void UpdatePos(math::scalar dt);
    void UpdateFront(math::scalar dt);

    math::xvec3 position_;
    math::xvec3 front_;
    math::xvec3 up_;
    math::xvec3 right_;
    math::xvec3 worldUp_;
    math::mat4 projectionMatrix_;
    math::mat4 viewMatrix_;
    math::mat4 viewProjectionMatrix_;

    math::scalar moveSpeed_;
    input::System<Camera> inputSys_;

    /********* Input handling ************/ //NOTE: Maybe move this to some mouse class?
    enum dir { fwd, bckwd, up, down, right, left, count };
    bool isMoving[dir::count] { false }; // Is moving to certain direction
    bool isLooking { false };
    bool firstMouse_ { true };
    math::vec2 lastPos_;
    math::vec2 newPos_;
    f32 yaw_ { 0.0f };
    f32 pitch_ { 0.0f };
};

}

