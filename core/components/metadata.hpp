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

#include "pool.hpp"

#include <string>

namespace reveal3d::core {

class Metadata {
public:
    using InitInfo = std::string;

    class Pool {
    public:
        std::string& name(id_t id) { return names_.at(id::index(id)); }

        std::string& comment(id_t id) { return comments_.at(id::index(id)); }

        std::string& date(id_t id) { return dates_.at(id::index(id)); }

        u32 count() { return names_.size(); }

        void add(id_t entity_id, InitInfo &init_info) {
            names_.emplace_back("Entity_" + std::to_string(entity_id));
            comments_.emplace_back();
            dates_.emplace_back("10/12/2024");  //TODO
            comments_.at(comments_.size() - 1U).reserve(1024);
        }

        void remove(u32 id) {
            names_.unordered_remove(id::index(id));
            comments_.unordered_remove(id::index(id));
            dates_.unordered_remove(id::index(id));
        }

    private:
        utl::vector<std::string> names_;
        utl::vector<std::string> comments_;
        utl::vector<std::string> dates_;
    };

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
