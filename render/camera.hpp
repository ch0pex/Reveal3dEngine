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
#include "math/math.hpp"
#include "window/window_info.hpp"

namespace reveal3d::render {

class Camera {
public:
    explicit Camera(const window::Resolution &res);

    void update(const Timer &timer);
    void resize(const window::Resolution &res);
    // TODO: Frustum

    [[nodiscard]] math::mat4 getProjectionMatrix() const { return projection_matrix_; }
    [[nodiscard]] math::mat4 getViewProjectionMatrix() const { return view_projection_matrix_; }
    [[nodiscard]] math::mat4 getViewMatrix() const { return view_matrix_; }

    /********* Input handling ************/
    void move(input::Action dir, input::type value);
    void setLooking(input::Action action, input::type value);
    void setNewMousePos(input::Action action, math::vec2 mouse_pos);
    void resetMouse() { first_mouse_ = true; }

private:
    void updatePos(math::scalar dt);
    void updateFront();

    math::xvec3 position_;
    math::xvec3 front_;
    math::xvec3 up_;
    math::xvec3 right_;
    math::xvec3 world_up_;
    math::mat4 projection_matrix_;
    math::mat4 view_matrix_;
    math::mat4 view_projection_matrix_;

    math::scalar move_speed_;

    /********* Input handling ************/ // NOTE: Maybe move this to some mouse class?
    enum Dir : u8 { Fwd, Bckwd, Up, Down, Right, Left, Count };
    std::array<bool, Dir::Count> is_moving_{false};
    bool is_looking_{false};
    bool first_mouse_{true};
    math::vec2 last_pos_{0.0f, 0.0f};
    math::vec2 new_pos_{0.0f, 0.0f};
    f32 yaw_{0.0f};
    f32 pitch_{0.0f};
};

} // namespace reveal3d::render
