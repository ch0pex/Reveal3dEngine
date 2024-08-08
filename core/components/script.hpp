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
    using PoolType = ScriptPool&;
    using InitInfo = std::unique_ptr<ScriptBase>;

    Script() = default;
    explicit Script(id_t id);
    Script(id_t id, InitInfo script);

    [[nodiscard]] INLINE bool IsAlive() const { return id_ != id::invalid; }
    [[nodiscard]] INLINE id_t Id() const { return id_; }

    void Begin();
    void Update(f32 dt);
    void DisableUpdate();
    void EnableUpdate();
    void Destroyed();

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
    /******************* Scripts Data *******************/
    std::vector<std::unique_ptr<ScriptBase>> scripts_;
};

}

