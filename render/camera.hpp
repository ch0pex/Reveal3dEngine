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

#include "common/timer.hpp"
#include "config/config.hpp"
#include "math/math.hpp"
#include "window/window_info.hpp"

namespace reveal3d::render {

class Camera {
public:
  explicit Camera(window::Resolution const& res) : resolution_(res) {
    using namespace input;

    add_handler_down(Action::CameraUp, {.callback = [this](Action const act, type const t) { move(act, t); }});
    add_handler_down(Action::CameraDown, {.callback = [this](Action const act, type const t) { move(act, t); }});
    add_handler_down(Action::CameraFwd, {.callback = [this](Action const act, type const t) { move(act, t); }});
    add_handler_down(Action::CameraLeft, {.callback = [this](Action const act, type const t) { move(act, t); }});
    add_handler_down(Action::CameraRight, {.callback = [this](Action const act, type const t) { move(act, t); }});
    add_handler_down(Action::CameraBackwd, {.callback = [this](Action const act, type const t) { move(act, t); }});

    add_handler_up(Action::CameraUp, {.callback = [this](Action const act, type const t) { move(act, t); }});
    add_handler_up(Action::CameraDown, {.callback = [this](Action const act, type const t) { move(act, t); }});
    add_handler_up(Action::CameraFwd, {.callback = [this](Action const act, type const t) { move(act, t); }});
    add_handler_up(Action::CameraLeft, {.callback = [this](Action const act, type const t) { move(act, t); }});
    add_handler_up(Action::CameraRight, {.callback = [this](Action const act, type const t) { move(act, t); }});
    add_handler_up(Action::CameraBackwd, {.callback = [this](Action const act, type const t) { move(act, t); }});

    add_handler_down(Action::CameraLook, {.callback = [this](Action const act, type const t) { setLooking(act, t); }});
    add_handler_up(Action::CameraLook, {.callback = [this](Action const act, type const t) { setLooking(act, t); }});
    add_mouse_handler(Action::CameraLook, {.mouse_callback = [this](Action const act, math::vec2 const pos) {
                        setNewMousePos(act, pos);
                      }});
    config_ = config::camera;
    resize(res);
  }

  [[nodiscard]] math::mat4 getProjectionMatrix() const { return projection_matrix_; }

  [[nodiscard]] math::mat4 getViewProjectionMatrix() const { return view_projection_matrix_; }

  [[nodiscard]] math::mat4 getViewMatrix() const { return view_matrix_; }

  [[nodiscard]] math::vec3 position() const { return {position_.x(), position_.y(), position_.z()}; }

  [[nodiscard]] f32 fov() const { return config_.fov; }

  void fov(f32 const new_fov) {
    config_.fov = new_fov;
    updateProjection();
  }

  [[nodiscard]] f32 farPlane() const { return config_.far_plane; }

  void farPlane(f32 const far_plane) {
    config_.far_plane = far_plane;
    updateProjection();
  }

  [[nodiscard]] f32 nearPlane() const { return config_.near_plane; }

  void nearPlane(f32 const near_plane) {
    config_.near_plane = near_plane;
    updateProjection();
  }

  [[nodiscard]] f32 moveSpeed() const { return config_.movement_speed; }

  void moveSpeed(f32 const speed) { config_.movement_speed = speed; }

  [[nodiscard]] f32 sensitivity() const { return config_.sensitivity; }

  void sensitivity(f32 const sens) { config_.sensitivity = sens; }

  void resize(window::Resolution const& res) {
    resolution_ = res;
    updateProjection();
  }

  /********* Input handling ************/

  void update(Timer const& timer) {
    updatePos(timer.frameTime());
    updateFront();
    view_matrix_            = look_at(position_, position_ + front_, up_);
    view_projection_matrix_ = view_matrix_ * projection_matrix_;
  }


  void move(input::Action const dir, input::type const value) { is_moving_.at(static_cast<u8>(dir)) = (value != 0); }

  void setLooking(input::Action const action, input::type const value) {
    is_looking_ = (value != 0);
    if (value == input::type::up) {
      first_mouse_ = true;
    }
  }

  void setNewMousePos(input::Action const action, math::vec2 const mouse_pos) {
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
  void updateProjection() {
    projection_matrix_ =
        math::perspective_fov(config_.fov, resolution_.aspect_ratio(), config_.near_plane, config_.far_plane);
  }

  void updatePos(math::scalar const dt) {
    u32 dirs                  = 0;
    math::scalar speed_factor = dt * config_.movement_speed;

    for (auto const dir: is_moving_) {
      if (dir)
        ++dirs;
    }

    speed_factor = (dirs > 1) ? speed_factor * 0.79F : speed_factor;

    if (is_moving_.at(Fwd))
      position_ += speed_factor * front_;
    if (is_moving_.at(Bckwd))
      position_ += speed_factor * -front_;
    if (is_moving_.at(Up))
      position_ += speed_factor * math::xvec4(config_.world_up);
    if (is_moving_.at(Down))
      position_ += speed_factor * -math::xvec4(config_.world_up);
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

    x_offset *= config_.sensitivity;
    y_offset *= config_.sensitivity;
    yaw_ += x_offset;
    pitch_ += y_offset;

    pitch_ = std::min(pitch_, 89.0F);
    pitch_ = std::max(pitch_, -89.0F);

    math::scalar const r = math::cos(math::radians(pitch_));
    math::xvec3 const new_front {
      r * math::cos(math::radians(yaw_)), r * math::sin(math::radians(yaw_)), math::sin(math::radians(pitch_))
    };

    front_    = normalize(new_front);
    right_    = normalize(math::cross(front_, config_.world_up));
    up_       = normalize(math::cross(right_, front_));
    last_pos_ = new_pos_;
  }

  // *** Data members ***
  math::xvec3 position_ {-6.F, 0.F, 2.F};
  math::xvec3 front_ {1, 0, 0};
  math::xvec3 up_ {0.F, 0.F, 1.F};
  math::xvec3 right_;
  math::mat4 projection_matrix_;
  math::mat4 view_matrix_;
  math::mat4 view_projection_matrix_;
  config::Camera config_;
  window::Resolution resolution_;


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
