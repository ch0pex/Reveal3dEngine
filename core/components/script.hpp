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
    using pool_type = Pool<script::Pool>;

    Script(id_t id) : id_(id) { }

    Script(id_t id, init_info script) { }

    [[nodiscard]] inline bool isAlive() const { return id_ != id::invalid; }
    [[nodiscard]] inline id_t id() const { return id_; }

    void begin() { pool.data().scripts.at(id::index(id_))->begin(); }


    void disableUpdate() { }

    void enableUpdate() { }

    void destroyed() { }

    void update() { pool.data().scripts.at(id::index(id_))->update(0.3f); }

    void enableBegin();
    void disableBegin();
    void enableDestroyed();
    void disableDestroyed();

private:
    inline static auto& pool = core::scene.componentPool<Script>();
    id_t id_ { id::invalid };
};

}

