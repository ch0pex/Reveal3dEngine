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
public:
    using init_info = std::string;
    using stored_in_gpu = std::false_type;

    void add(id_t entity_id, init_info &init_info) {
//        names_.emplace_back("Entity_" + std::to_string(entity_id));
//        comments_.emplace_back();
//        dates_.emplace_back("10/12/2024");  //TODO
//        comments_.at(comments_.size() - 1U).reserve(1024);
    }

    void remove(u32 id) {
//        names_.unordered_remove(id::index(id));
//        comments_.unordered_remove(id::index(id));
//        dates_.unordered_remove(id::index(id));
    }
    u32 count() { return scripts.size(); }
private:
    utl::vector<Flag> flags;
    utl::vector<std::unique_ptr<ScriptBase>> scripts;
};

}