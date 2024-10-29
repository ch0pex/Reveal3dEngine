/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file transform_pool.hpp
 * @version 1.0
 * @date 29/10/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once


class Script;

namespace reveal3d::core::script {

class ScriptBase {
public:
    virtual void begin() = 0;
    virtual void update(f32 dt) = 0;
    virtual void destroyed() = 0;
};

struct Flag {
    u8 begin   : 1; // begin function active
    u8 update  : 1; // update function active
    u8 destroy : 1; // destroy function active
};

struct Data {
    Flag flags;
    std::unique_ptr<ScriptBase> scripts;
};


class Pool {
    u32 count() { return scripts.size(); }
    utl::vector<Flag> flags;
    utl::vector<std::unique_ptr<ScriptBase>> scripts;
};

}