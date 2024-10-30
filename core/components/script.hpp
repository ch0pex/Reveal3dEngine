/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file script.hpp
 * @version 1.0
 * @date 01/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "core/scene.hpp"

namespace reveal3d::core {


class Script {
public:
    using init_info = std::unique_ptr<script::ScriptBase>;
    using pool_type = script::Pool;

    constexpr Script(id_t id) : id_(id) { }

    constexpr Script(id_t id, init_info script) { }

    [[nodiscard]] constexpr bool isAlive() const { return id_ != id::invalid; }
    [[nodiscard]] constexpr id_t id() const { return id_; }

    void begin() { pool.scripts(id_)->begin(); }


    void disableUpdate() { }

    void enableUpdate() { }

    void destroyed() { }

    void update() { pool.scripts(id_)->update(0.3f); }

    void data();

    void enableBegin();
    void disableBegin();
    void enableDestroyed();
    void disableDestroyed();

private:
    inline static GenericPool<pool_type>& pool = core::scene.componentPool<Script>();

    id_t id_ { id::invalid };
};

}

