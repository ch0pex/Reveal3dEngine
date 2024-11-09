/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file geometry.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Geometry entity component
 */

#pragma once

#include "core/scene.hpp"

#include <string>

namespace reveal3d::core {

class Metadata {
public:
    using init_info = std::string;
    using pool_type = metadata::Pool;

    constexpr Metadata() : id_(id::invalid) {}

    constexpr Metadata(id_t id) : id_(id) { }

    [[nodiscard]] constexpr bool isAlive() const { return id_ != id::invalid; }

    [[nodiscard]] constexpr id_t id() const { return id_; }

    [[nodiscard]] std::string& name() const { return scene.componentPool<Metadata>().name(id_); }

    [[nodiscard]] std::string& date() const { return scene.componentPool<Metadata>().date(id_); }

    [[nodiscard]] std::string& comment() const { return scene.componentPool<Metadata>().comment(id_); }
private:

    id_t id_;
};

}
