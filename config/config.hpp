/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file config.hpp
 * @version 1.0
 * @date 21/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "math/math.hpp"

#include <toml++/toml.hpp>

namespace reveal3d {

namespace detail {

struct Scene {
    static inline math::vec4 clearColor = {0.2F, 0.2F, 0.2F, 1.0F};
    static inline bool showGrid = true;
};

struct Graphics {
    static inline u8 max_framerate = 0;
    static inline bool vsync = true;
};

struct Lighting {
    static inline f32 ambient_light_intensity = 1.0F;
    static inline math::vec4 ambient_color = {1.0F, 1.0F, 1.0F, 1.0F};
};

struct Window {};

struct Camera {
    static inline f32 movement_speed = 1.0F;
    static inline math::vec3 position = {0.0F, 0.0F, 0.0F};
    static inline math::vec3 rotation = {0.0F, 0.0F, 0.0F};
    static inline math::vec3 front = {0.0F, 0.0F, -1.0F};
    static inline math::vec3 up = {0.0F, 1.0F, 0.0F};
    static inline f32 fov = 45.0F;
    static inline f32 near_plane = 0.1F;
    static inline f32 far_plane = 100.0F;
};

} // namespace detail

struct Config {
    static inline detail::Scene scene;
    static inline detail::Graphics graphics;
    static inline detail::Lighting lighting;
    static inline detail::Camera camera;
};

} // namespace reveal3d
