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

class ScriptPool;

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

    Script() = default;
    explicit Script(id_t id);
    Script(id_t id, InitInfo script);

    [[nodiscard]] INLINE bool IsAlive() const { return id_ != id::invalid; }
    [[nodiscard]] INLINE id_t Id() const { return id_; }

    void Begin();
    void Update(f32 dt);
    void Destroyed();

    void EnableUpdate();
    void DisableUpdate();
    void EnableBegin();
    void DisableBegin();
    void EnableDestroyed();
    void DisableDestroyed();

private:
    [[nodiscard]] static ScriptPool& Pool();
    id_t id_ { id::invalid };
};

class ScriptPool : public Pool<Script> {
public:
    Script AddComponent(id_t id);
    Script AddComponent(id_t id, Script::InitInfo initInfo);
    void RemoveComponent(id_t id) override;
    void Update() override;

    [[nodiscard]] INLINE u32  Count() override { return scripts_.size(); };

private:
    friend class Script;
    /******************* Scripts Data *******************/
    utl::vector<std::unique_ptr<ScriptBase>> scripts_;
    utl::vector<Script::Flag>                flags_;
};

}

