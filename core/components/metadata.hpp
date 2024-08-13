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
#include "common/common.hpp"

#include <string>

namespace reveal3d::core {

class MetadataPool;

class Metadata {
public:
    using InitInfo = std::string;

    Metadata() = default;
    explicit Metadata(id_t id);

    std::string& Name();
    std::string& Comment();
    std::string& Date();

    [[nodiscard]] INLINE bool IsAlive() const { return id_ != id::invalid; }
    [[nodiscard]] INLINE id_t Id() const { return id_; }
private:
    [[nodiscard]] static MetadataPool& Pool();
    id_t id_;
};

class MetadataPool : public Pool<Metadata> {
public:
    Metadata AddComponent(id_t id) override;
    Metadata AddComponent(id_t id, Metadata::InitInfo&& initInfo);
    void RemoveComponent(id_t id) override;
    void Update() override;
    INLINE u32  Count() override { return names_.size(); }


private:
    friend class Metadata;
    utl::vector<std::string> names_;
    utl::vector<std::string> comments_;
    utl::vector<std::string> dates_;
};

}
