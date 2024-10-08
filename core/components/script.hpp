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

#include <functional>


namespace reveal3d::core {

class ScriptBase {
public:
    virtual void Begin() = 0;
    virtual void Update(float dt) = 0;
    virtual void Destroyed() = 0;
};

class Script {
public:
    using InitInfo = std::unique_ptr<ScriptBase>;

    struct Flag { 
        u8 begin   : 1; // Begin function active
        u8 update  : 1; // Update function active
        u8 destroy : 1; // Destroy function active
    };

    struct Data {
        u32 Count() { return scripts.size(); }
        utl::vector<Flag> flags;
        utl::vector<std::unique_ptr<ScriptBase>> scripts;
    };

    Script() = default;
    explicit Script(id_t id);
    Script(id_t id, InitInfo script);

    [[nodiscard]] INLINE bool IsAlive() const { return id_ != id::invalid; }
    [[nodiscard]] INLINE id_t Id() const { return id_; }

    void Begin();
    void Update();
    void Destroyed();

    void EnableUpdate();
    void DisableUpdate();
    void EnableBegin();
    void DisableBegin();
    void EnableDestroyed();
    void DisableDestroyed();

private:
    [[nodiscard]] static Pool<Script>& Pool();
    id_t id_ { id::invalid };
};

template<>
Script Pool<Script>::AddComponent(id_t id) {
    return Script();
}

template<>
Script Pool<Script>::AddComponent(id_t id, Script::InitInfo&& initInfo) {
    return Script();
}

template<>
void Pool<Script>::RemoveComponent(id_t id) {

}

}

