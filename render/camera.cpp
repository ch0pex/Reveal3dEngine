/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file camera.cpp
 * @version 1.0
 * @date 01/03/2024
 * @brief Reveal3D Camera 
 *
 * Visualization Camera Class 
 */

#include "camera.hpp"


namespace reveal3d::render{

Camera::Camera(const window::Resolution &res) :
    projection_matrix_(math::perspective_fov(65.f, res.aspect_ratio, 0.1f, 100.0f)),
        position_(-6.0f, 0.0f, 2.0f),
    move_speed_(5.0f), front_(1,0,0), world_up_(0,0,1), up_(0,0,1), right_(0,-1,0)
{
    using namespace input;
    add_handler_down(Action::CameraUp, {[this](Action act, type t) { move(act, t); }});
    add_handler_down(Action::CameraDown, {[this](Action act, type t) { move(act, t); }});
    add_handler_down(Action::CameraFwd, {[this](Action act, type t) { move(act, t); }});
    add_handler_down(Action::CameraLeft, {[this](Action act, type t) { move(act, t); }});
    add_handler_down(Action::CameraRight, {[this](Action act, type t) { move(act, t); }});
    add_handler_down(Action::CameraBackwd, {[this](Action act, type t) { move(act, t); }});

    add_handler_up(Action::CameraUp, {[this](Action act, type t) { move(act, t); }});
    add_handler_up(Action::CameraDown, {[this](Action act, type t) { move(act, t); }});
    add_handler_up(Action::CameraFwd, {[this](Action act, type t) { move(act, t); }});
    add_handler_up(Action::CameraLeft, {[this](Action act, type t) { move(act, t); }});
    add_handler_up(Action::CameraRight, {[this](Action act, type t) { move(act, t); }});
    add_handler_up(Action::CameraBackwd, {[this](Action act, type t) { move(act, t); }});

    add_handler_down(Action::CameraLook, {[this](Action act, type t) { setLooking(act, t); }});
    add_handler_up(Action::CameraLook, {[this](Action act, type t) { setLooking(act, t); }});
    add_mouse_handler(Action::CameraLook,
                      {.mouse_callback = [this](Action act, math::vec2 pos) { setNewMousePos(act, pos); }});
}


void Camera::update(const Timer &timer) {
    updatePos(timer.frameTime());
    updateFront();
    view_matrix_ = math::look_at(position_, position_ + front_, up_);
    view_projection_matrix_ = view_matrix_ * projection_matrix_;
}

void Camera::resize(const window::Resolution &res) {
    projection_matrix_ = math::perspective_fov(65.f, res.aspect_ratio, 0.1f, 100.0f);
}

void Camera::move(const input::Action dir, const input::type value) {
    is_moving_[dir] = value;
//    logger(logDEBUG) << "EyePos: " << position_.x() << ", " << position_.y() << ", " << position_.z();
}

void Camera::setLooking(const input::Action action, const input::type value) {
//    input::cursor::shouldClip = (value == input::type::up) ? false : true;
is_looking_ = (value != 0);
    if (value == input::type::up) {
        first_mouse_ = true;
    }
}

void Camera::setNewMousePos(const input::Action action, const math::vec2 mouse_pos) {
    if (!is_looking_) return;
    if (first_mouse_) {
        last_pos_ = mouse_pos;
        first_mouse_ = false;
    }
    new_pos_ = mouse_pos;

}

void Camera::updatePos(math::scalar dt) {
    u32 dirs = 0;
    math::scalar speed_factor = dt * move_speed_;

    for(auto& dir : is_moving_) {
        if (dir)  ++dirs;
    }

    speed_factor = (dirs > 1) ? speed_factor * 0.79f : speed_factor;

    if (is_moving_[Dir::Fwd])      position_ += speed_factor * front_;
    if (is_moving_[Dir::Bckwd])    position_ += speed_factor * -front_;
    if (is_moving_[Dir::Up])       position_ += speed_factor * world_up_;
    if (is_moving_[Dir::Down])     position_ += speed_factor * -world_up_;
    if (is_moving_[Dir::Right])    position_ += speed_factor * right_;
    if (is_moving_[Dir::Left])     position_ += speed_factor * -right_;
}

void Camera::updateFront() {
    if (!is_looking_) return;
    math::xvec3 new_front;
    f32 x_offset = new_pos_.x - last_pos_.x;
    f32 y_offset = last_pos_.y - new_pos_.y;
    constexpr f32 sensitivity = 50.0f * 0.001f; //TODO: move this to config

    x_offset *= sensitivity;
    y_offset *= sensitivity;
    yaw_ += x_offset;
    pitch_ += y_offset;

    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;

    const math::scalar r = math::cos(math::radians(pitch_));
    new_front = {
            r * math::cos(math::radians(yaw_)),
            r * math::sin(math::radians(yaw_)),
            math::sin(math::radians(pitch_))
    };

//    logger(logDEBUG) << newFront.x() << ", " << newFront.y() << ", " << newFront.z();
    front_ = math::normalize(new_front);
    right_ = math::normalize(math::cross(front_, world_up_));
    up_ = math::normalize(math::cross(right_, front_));
    last_pos_ = new_pos_;
//    logger(logDEBUG) << front_.x() << ", " << front_.y() << ", " << front_.z();
}

}