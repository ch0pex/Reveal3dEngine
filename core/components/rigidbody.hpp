/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file rigidbody_pool.hpp
 * @version 1.0
 * @date 31/10/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "core/scene.hpp"

namespace reveal3d::core {

class Rigidbody {
public:
    using init_info = std::string;
    using pool_type = rigidbody::Pool;

    constexpr Rigidbody(id_t id) : id_(id) {}

    constexpr Rigidbody() : id_(id::invalid) {}

    [[nodiscard]] constexpr bool isAlive() const { return id_ != id::invalid; }

    [[nodiscard]] constexpr id_t id() const { return id_; }

private:
    id_t id_;
};

} // namespace reveal3d::core
