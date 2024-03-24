/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file camera.cpp
 * @version 1.0
 * @date 01/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "camera.hpp"


namespace reveal3d::render{

Camera::Camera(const window::Resolution &res) :
        projectionMatrix_(XMMatrixPerspectiveFovLH(XMConvertToRadians(65.f), res.aspectRatio, 0.1f, 100.0f)),
        position_(-6.0f, 0.0f, 0.0f),
        moveSpeed_(15.0f), front_(1,0,0),
        worldUp_(0,0,1), up_(0,0,1), right_(0,1,0)
{
    inputSys_.AddHandlerDown(input::action::camera_up, {&Camera::Move, nullptr, this});
    inputSys_.AddHandlerDown(input::action::camera_down, {&Camera::Move, nullptr, this});
    inputSys_.AddHandlerDown(input::action::camera_fwd, {&Camera::Move, nullptr, this});
    inputSys_.AddHandlerDown(input::action::camera_left, {&Camera::Move, nullptr,  this});
    inputSys_.AddHandlerDown(input::action::camera_right, {&Camera::Move, nullptr, this});
    inputSys_.AddHandlerDown(input::action::camera_backwd, {&Camera::Move, nullptr, this});

    inputSys_.AddHandlerUp(input::action::camera_up, {&Camera::Move, nullptr, this});
    inputSys_.AddHandlerUp(input::action::camera_down, {&Camera::Move, nullptr, this});
    inputSys_.AddHandlerUp(input::action::camera_fwd, {&Camera::Move, nullptr, this});
    inputSys_.AddHandlerUp(input::action::camera_left, {&Camera::Move, nullptr, this});
    inputSys_.AddHandlerUp(input::action::camera_right, {&Camera::Move, nullptr, this});
    inputSys_.AddHandlerUp(input::action::camera_backwd, {&Camera::Move, nullptr, this});

    inputSys_.AddHandlerDown(input::action::camera_look, {&Camera::SetLooking, nullptr, this});
    inputSys_.AddHandlerUp(input::action::camera_look, {&Camera::SetLooking, nullptr, this});
    inputSys_.AddMouseHandler(input::action::camera_look, {nullptr, &Camera::SetNewMousePos, this});
}


void Camera::Update(const Timer& timer) {
    UpdatePos(timer.DeltaTime());
    UpdateFront(timer.DeltaTime());
    viewMatrix_ = math::LookAt(position_, position_ + front_, up_);
    viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;
}

void Camera::Resize(const window::Resolution &res) {
    projectionMatrix_ = math::PerspectiveFov(65.f, res.aspectRatio, 0.1f, 100.0f);
}

void Camera::Move(const input::action dir, const input::type value) {
    isMoving[dir] = value;
}

void Camera::SetLooking(const input::action action, const input::type value) {
    isLooking = value;
    if (value == input::type::up) firstMouse_ = true;
}

void Camera::SetNewMousePos(const input::action action, const math::vec2 mousePos) {
    if (!isLooking) return;
    if (firstMouse_) {
        lastPos_ = mousePos;
        firstMouse_ = false;
    }

    newPos_ = mousePos;

}

void Camera::UpdatePos(math::scalar dt) {
    if(isMoving[dir::fwd])      position_ += dt * moveSpeed_ * front_;
    if(isMoving[dir::bckwd])    position_ += dt * moveSpeed_ * -front_;
    if(isMoving[dir::up])       position_ += dt * moveSpeed_ * worldUp_;
    if(isMoving[dir::down])     position_ += dt * moveSpeed_ * -worldUp_;
    if(isMoving[dir::right])     position_ += dt * moveSpeed_ * right_;
    if(isMoving[dir::left])    position_ += dt * moveSpeed_ * -right_;
}

void Camera::UpdateFront(math::scalar dt) {
    if (!isLooking) return;
    math::xvec3 newFront;
    f32 xOffset =  newPos_.x - lastPos_.x;
    f32 yOffset = lastPos_.y - newPos_.y;
    f32 sensitivity = 200.0f; //TODO: Move this to config

    xOffset *= sensitivity * dt;
    yOffset *= sensitivity * dt;

    yaw_ += xOffset;
    pitch_ += yOffset;

    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;

    const math::scalar r = math::Cos(math::Radians(pitch_));
    newFront = {
            r * math::Cos(math::Radians(yaw_)),
            r * math::Sin(math::Radians(yaw_)),
            math::Sin(math::Radians(pitch_))
    };

    front_ = math::Normalize(newFront);
    right_ = math::Normalize(math::Cross(front_, worldUp_));
    up_ = math::Normalize(math::Cross(right_, front_));

    lastPos_ = newPos_;
}


}