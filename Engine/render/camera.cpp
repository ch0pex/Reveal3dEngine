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
        moveSpeed_(15.0f), lookDir_(1,0,0),
        upDirection_(0,0,1)
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
}


void Camera::Update(const Timer& timer) {
    UpdatePos(timer.DeltaTime());
    viewMatrix_ = math::LookAt(position_, position_ + lookDir_, upDirection_);
    viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;
}

void Camera::Resize(const window::Resolution &res) {
    projectionMatrix_ = math::PerspectiveFov(65.f, res.aspectRatio, 0.1f, 100.0f);
}

void Camera::Move(input::action dir, input::type inputType) {
    isMoving[dir] = inputType;
}

void Camera::UpdatePos(math::scalar dt) {
    if(isMoving[dir::fwd])      position_ += dt * moveSpeed_ * math::direction[dir::fwd];
    if(isMoving[dir::bckwd])    position_ += dt * moveSpeed_ * math::direction[dir::bckwd];
    if(isMoving[dir::up])       position_ += dt * moveSpeed_ * math::direction[dir::up];
    if(isMoving[dir::down])     position_ += dt * moveSpeed_ * math::direction[dir::down];
    if(isMoving[dir::left])     position_ += dt * moveSpeed_ * math::direction[dir::left];
    if(isMoving[dir::right])    position_ += dt * moveSpeed_ * math::direction[dir::right];
}


}