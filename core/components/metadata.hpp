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

    struct Data {
        Data(std::string& name,std::string& comm, std::string& date) : name(name.data()), comment(comm.data()), date(date.data()) {}
        char * name;
        char * comment;
        char * date;
    };

    constexpr Metadata() : id_(id::invalid) {}

    constexpr Metadata(id_t id) : id_(id) { }

    [[nodiscard]] constexpr bool isAlive() const { return id_ != id::invalid; }

    [[nodiscard]] constexpr id_t id() const { return id_; }

    std::string& name() { return scene.componentPool<Metadata>().name(id_); }

    std::string& date() { return scene.componentPool<Metadata>().date(id_); }

    std::string& comment() { return scene.componentPool<Metadata>().comment(id_); }

    Data data() { return {name(), comment(), date()}; }

private:

    id_t id_;
};

}
