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

#include "pool.hpp"
#include "common/vector.hpp"


namespace reveal3d::core {

class ScriptBase {
public:
    virtual void begin() = 0;
    virtual void update(f32 dt) = 0;
    virtual void destroyed() = 0;
};

class Script {
public:
    using InitInfo = std::unique_ptr<ScriptBase>;

    struct Flag {
        u8 begin   : 1; // begin function active
        u8 update  : 1; // update function active
        u8 destroy : 1; // destroy function active
    };

    struct Pool {
        u32 count() { return scripts.size(); }
        utl::vector<Flag> flags;
        utl::vector<std::unique_ptr<ScriptBase>> scripts;
    };

    struct Data {
       Flag flags;
       std::unique_ptr<ScriptBase> scripts;
    };

    Script() = default;
    explicit Script(id_t id);
    Script(id_t id, InitInfo script);

    [[nodiscard]] inline bool isAlive() const { return id_ != id::invalid; }
    [[nodiscard]] inline id_t id() const { return id_; }

    void begin();
    void update();
    Data data();
    void destroyed();

    void enableUpdate();
    void disableUpdate();
    void enableBegin();
    void disableBegin();
    void enableDestroyed();
    void disableDestroyed();

private:
    id_t id_ { id::invalid };
};

template<>
inline Script Pool<Script>::addComponent(id_t id) {
    return Script();
}

template<>
inline Script Pool<Script>::addComponent(id_t id, Script::InitInfo &&init_info) {
    return Script();
}

template<>
inline void Pool<Script>::removeComponent(id_t id) {

}

}

