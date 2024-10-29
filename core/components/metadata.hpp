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

#include "common/common.hpp"

#include <string>

namespace reveal3d::core {

class Metadata {
public:
    using InitInfo = std::string;

    struct Data {
        Data(std::string& name,std::string& comm, std::string& date) : name(name.data()), comment(comm.data()), date(date.data()) {}
        char * name;
        char * comment;
        char * date;
    };

    Metadata() : id_(id::invalid) {}

    explicit Metadata(id_t id);

    std::string& name();

    std::string& comment();

    std::string& date();

    [[nodiscard]] bool isAlive() const { return id_ != id::invalid; }

    [[nodiscard]] id_t id() const { return id_; }

    Data data();

private:
    id_t id_;
};

}
