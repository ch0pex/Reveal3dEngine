/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file camera.hpp
 * @version 1.0
 * @date 01/03/2024
 * @brief Camera 
 *
 * Visualization camera 
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
    [[nodiscard]] INLINE math::mat4 const GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
    [[nodiscard]] INLINE math::mat4 GetViewMatrix() const { return viewMatrix_; }

    /********* Input handling ************/
    void Move(input::action dir, input::type value);
    void SetLooking(input::action action, input::type value);
    void SetNewMousePos(input::action action, math::vec2 mousePos);

    INLINE void ResetMouse() { firstMouse_ = true; }

private:
    void UpdatePos(math::scalar dt);
    void UpdateFront();

    math::xvec3 position_;
    math::xvec3 front_;
    math::xvec3 up_;
    math::xvec3 right_;
    math::xvec3 worldUp_;
    math::mat4 projectionMatrix_;
    math::mat4 viewMatrix_;
    math::mat4 viewProjectionMatrix_;

    math::scalar moveSpeed_;
    input::System inputSys_;

    /********* Input handling ************/ //NOTE: Maybe move this to some mouse class?
    enum dir : u8 { fwd, bckwd, up, down, right, left, count };
    bool isMoving_[dir::count] { false };
    bool isLooking_{ false };
    bool firstMouse_ { true };
    math::vec2 lastPos_ {0.0f, 0.0f};
    math::vec2 newPos_ {0.0f, 0.0f};
    f32 yaw_ { 0.0f };
    f32 pitch_ { 0.0f };
};

}

