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

    struct Data {
        u32 Count() { return names.size(); }
        utl::vector<std::string> names;
        utl::vector<std::string> comments;
        utl::vector<std::string> dates;
    };

    Metadata() = default;
    explicit Metadata(id_t id);

    std::string& Name();
    std::string& Comment();
    std::string& Date();

    [[nodiscard]] INLINE bool IsAlive() const { return id_ != id::invalid; }
    [[nodiscard]] INLINE id_t Id() const { return id_; }
    void Update();
private:
    [[nodiscard]] static Pool<Metadata>& Pool();
    id_t id_;
};

template<>
inline Metadata Pool<Metadata>::AddComponent(id_t id) {
    const id_t metadata_id { id_factory_.New(id::index(id)) };

    data_.names.emplace_back("Entity" + std::to_string(id));
    data_.comments.emplace_back();
    data_.dates.emplace_back("10/12/2024");  //TODO
    data_.comments.at(data_.comments.size() - 1U).reserve(1024);
    Add(id::index(id), metadata_id);

    return Metadata(metadata_id);
}

template<>
inline Metadata Pool<Metadata>::AddComponent(id_t id, Metadata::InitInfo &&initInfo) {
    const id_t metadata_id { id_factory_.New() };

    data_.names.emplace_back(std::move(initInfo));
    data_.comments.emplace_back();
    data_.dates.emplace_back("10/12/2024");  //TODO
    Add(id::index(id), metadata_id);

    return Metadata(metadata_id);
}

template<>
inline void Pool<Metadata>::RemoveComponent(id_t id) {
    id_t idx { id::index(id) };
    data_.names.unordered_remove(idx);
    data_.comments.unordered_remove(idx);
    data_.dates.unordered_remove(idx);
    id_factory_.Remove(id);
}

}
