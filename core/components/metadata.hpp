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
    using PoolType = TransformPool&;
    using InitInfo = std::string;

    Transform() = default;
    explicit Transform(id_t id);

    [[nodiscard]] INLINE bool IsAlive() const { return id_ != id::invalid; }
    [[nodiscard]] INLINE id_t Id() const { return id_; }
private:
    id_t id_;
};

class MetadataPool : public Pool<Metadata> {
public:
    Metadata AddComponent(id_t id);
    Metadata AddComponent(id_t id, Metadata::InitInfo&& initInfo);
    void RemoveComponent(id_t id) override;
    void Update() override;

    INLINE u32  Count()  override           { return transform_data_.size(); }


private:
    friend class Metadata;
    utl::vector<std::string> names_;
    utl::vector<std::string> comments_;
    utl::vector<std::string> date_;
}

}
