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

#include <core/config/config.hpp>


#include "common/timer.hpp"
#include "math/math.hpp"
#include "window/window_info.hpp"

namespace reveal3d::render {

class Camera {
public:
  explicit Camera(const window::Resolution& res) :
    projection_matrix_(math::perspective_fov(config::camera.fov, res.aspect_ratio, config::camera.near_plane, config::camera.far_plane)) {
    using namespace input;
    add_handler_down(Action::CameraUp, {.callback = [this](const Action act, const type t) { move(act, t); }});
    add_handler_down(Action::CameraDown, {.callback = [this](const Action act, const type t) { move(act, t); }});
    add_handler_down(Action::CameraFwd, {.callback = [this](const Action act, const type t) { move(act, t); }});
    add_handler_down(Action::CameraLeft, {.callback = [this](const Action act, const type t) { move(act, t); }});
    add_handler_down(Action::CameraRight, {.callback = [this](const Action act, const type t) { move(act, t); }});
    add_handler_down(Action::CameraBackwd, {.callback = [this](const Action act, const type t) { move(act, t); }});

    add_handler_up(Action::CameraUp, {.callback = [this](const Action act, const type t) { move(act, t); }});
    add_handler_up(Action::CameraDown, {.callback = [this](const Action act, const type t) { move(act, t); }});
    add_handler_up(Action::CameraFwd, {.callback = [this](const Action act, const type t) { move(act, t); }});
    add_handler_up(Action::CameraLeft, {.callback = [this](const Action act, const type t) { move(act, t); }});
    add_handler_up(Action::CameraRight, {.callback = [this](const Action act, const type t) { move(act, t); }});
    add_handler_up(Action::CameraBackwd, {.callback = [this](const Action act, const type t) { move(act, t); }});

    add_handler_down(Action::CameraLook, {.callback = [this](const Action act, const type t) { setLooking(act, t); }});
    add_handler_up(Action::CameraLook, {.callback = [this](const Action act, const type t) { setLooking(act, t); }});
    add_mouse_handler(Action::CameraLook, {.mouse_callback = [this](const Action act, const math::vec2 pos) {
                        setNewMousePos(act, pos);
                      }});
  }

  [[nodiscard]] math::mat4 getProjectionMatrix() const { return projection_matrix_; }
  [[nodiscard]] math::mat4 getViewProjectionMatrix() const { return view_projection_matrix_; }
  [[nodiscard]] math::mat4 getViewMatrix() const { return view_matrix_; }

  /********* Input handling ************/

  void update(const Timer& timer) {
    updatePos(timer.frameTime());
    updateFront();
    view_matrix_            = look_at(position_, position_ + front_, up_);
    view_projection_matrix_ = view_matrix_ * projection_matrix_;
  }

  void resize(const window::Resolution& res) {
    projection_matrix_ = math::perspective_fov(65.F, res.aspect_ratio, 0.1F, 100.0F);
  }

  void move(const input::Action dir, const input::type value) {
    is_moving_.at(static_cast<u8>(dir)) = (value != 0);
    logger(LogDebug) << "EyePos: " << position_.x() << ", " << position_.y() << ", " << position_.z();
  }

  void setLooking(const input::Action action, const input::type value) {
    //    input::cursor::shouldClip = (value == input::type::up) ? false : true;
    is_looking_ = (value != 0);
    if (value == input::type::up) {
      first_mouse_ = true;
    }
  }

  void setNewMousePos(const input::Action action, const math::vec2 mouse_pos) {
    if (!is_looking_) {
      return;
    }
    if (first_mouse_) {
      last_pos_    = mouse_pos;
      first_mouse_ = false;
    }
    new_pos_ = mouse_pos;
  }

  void resetMouse() { first_mouse_ = true; }

private:
  void updatePos(const math::scalar dt) {
    u32 dirs                  = 0;
    math::scalar speed_factor = dt * config::camera.movement_speed;

    for (const auto dir: is_moving_) {
      if (dir)
        ++dirs;
    }

    speed_factor = (dirs > 1) ? speed_factor * 0.79F : speed_factor;

    if (is_moving_.at(Fwd))
      position_ += speed_factor * front_;
    if (is_moving_.at(Bckwd))
      position_ += speed_factor * -front_;
    if (is_moving_.at(Up))
      position_ += speed_factor * math::xvec4(config::camera.world_up);
    if (is_moving_.at(Down))
      position_ += speed_factor * -math::xvec4(config::camera.world_up);
    if (is_moving_.at(Right))
      position_ += speed_factor * right_;
    if (is_moving_.at(Left))
      position_ += speed_factor * -right_;
  }

  void updateFront() {
    if (!is_looking_)
      return;
    f32 x_offset = new_pos_.x - last_pos_.x;
    f32 y_offset = last_pos_.y - new_pos_.y;

    x_offset *= config::camera.sensitivity;
    y_offset *= config::camera.sensitivity;
    yaw_ += x_offset;
    pitch_ += y_offset;

    pitch_ = std::min(pitch_, 89.0F);
    pitch_ = std::max(pitch_, -89.0F);

    const math::scalar r        = math::cos(math::radians(pitch_));
    const math::xvec3 new_front {
      r * math::cos(math::radians(yaw_)), r * math::sin(math::radians(yaw_)), math::sin(math::radians(pitch_))
    };

    //    logger(LogDebug) << newFront.x() << ", " << newFront.y() << ", " << newFront.z();
    front_    = normalize(new_front);
    right_    = normalize(math::cross(front_, config::camera.world_up));
    up_       = normalize(math::cross(right_, front_));
    last_pos_ = new_pos_;
    logger(LogDebug) << front_.x() << ", " << front_.y() << ", " << front_.z();
  }

  math::xvec3 position_ {-6.F, 0.F, 2.F};
  math::xvec3 front_ {1, 0, 0};
  math::xvec3 up_ {0.F, 0.F, 1.F};
  math::xvec3 right_;
  math::mat4 projection_matrix_;
  math::mat4 view_matrix_;
  math::mat4 view_projection_matrix_;

  /********* Input handling ************/ // NOTE: Maybe move this to some mouse class?
  enum Dir : u8 { Fwd, Bckwd, Up, Down, Right, Left, Count };
  std::array<bool, Dir::Count> is_moving_ {false};
  bool is_looking_ {false};
  bool first_mouse_ {true};
  math::vec2 last_pos_ {0.0F, 0.0F};
  math::vec2 new_pos_ {0.0F, 0.0F};
  f32 yaw_ {0.0F};
  f32 pitch_ {0.0F};
};

} // namespace reveal3d::render
