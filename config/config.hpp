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
    math::vec4 clearColor = {0.2F, 0.2F, 0.2F, 1.0F};
    bool showGrid = true;
};

struct Graphics {
    u8 max_framerate = 0;
    bool vsync = true;
};

struct Lighting {
    f32 ambient_light_intensity = 1.0F;
    math::vec4 ambient_color = math::vec4(1.0F, 1.0F, 1.0F, 1.0F);

    void setFromConfig(auto node_view) {
        ambient_color = node_view["ambient_color"].value_or(ambient_color);
        ambient_light_intensity = node_view["ambient_light_intensity"].value_or(ambient_light_intensity);
    }
};

struct Window {};

struct Camera {
    f32 movement_speed = 1.0F;
    math::vec3 position = {0.0F, 0.0F, 0.0F};
    math::vec3 rotation = {0.0F, 0.0F, 0.0F};
    math::vec3 front = {0.0F, 0.0F, -1.0F};
    math::vec3 up = {0.0F, 1.0F, 0.0F};
    f32 fov = 45.0F;
    f32 near_plane = 0.1F;
    f32 far_plane = 100.0F;

    void setFromConfig(auto node_view) {
        movement_speed = node_view["movement_speed"].value_or(movement_speed);
        position = node_view["position"].value_or(position);
        rotation = node_view["rotation"].value_or(rotation);
        front = node_view["front"].value_or(front);
        up = node_view["up"].value_or(up);
        fov = node_view["fov"].value_or(fov);
        near_plane = node_view["near_plane"].value_or(near_plane);
        far_plane = node_view["far_plane"].value_or(far_plane);
    }
};

} // namespace detail

struct Config {
    static inline detail::Scene scene;
    static inline detail::Graphics graphics;
    static inline detail::Lighting lighting;
    static inline detail::Camera camera;

    void loadFromToml(const std::string_view config_file) {
        toml::table tbl;
        try {
            tbl = toml::parse_file(config_file);
            std::cout << tbl << "\n";
        }
        catch (const toml::parse_error& err) {
            std::cerr << "Parsing failed:\n" << err << "\n";
            std::cerr << "Using default settings:\n";
            return;
        }
    }
};

} // namespace reveal3d
