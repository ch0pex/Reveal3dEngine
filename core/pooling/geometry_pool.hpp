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

#include "common/common.hpp"
#include "math/math.hpp"
#include "render/mesh.hpp"
#include <span>

namespace reveal3d::core::geometry {

class Pool {
public:
    using init_info = render::Mesh;
    using stored_in_gpu = std::true_type;

    render::Material& material(id_t id) { return materials_.at(id::index(id)); }

    render::Mesh& mesh(id_t id) { return meshes_.at(id::index(id)); }

    std::span<render::SubMesh> subMeshes(id_t id) { return std::span {sub_meshes_.begin() + id::index(id), sub_meshes_.begin() + id::index(id) + 1}; }

protected:
    u32 countData() { return meshes_.size(); }

    void addData(id_t entity_id, init_info &init_info) {
        materials_.emplace_back();
        sub_meshes_.emplace_back(render::SubMesh {
                .shader = render::Opaque,
                .vertex_pos = 0,
                .index_pos = 0,
                .index_count = static_cast<u32>(init_info.indices.size()),
                .visible = true,
        });
        meshes_.push_back(std::move(init_info));
    }

    void removeData(u32 id) {
        materials_.unordered_remove(id::index(id));
        meshes_.unordered_remove(id::index(id));
        sub_meshes_.unordered_remove(id::index(id));
    }
private:
    utl::vector<render::Material> materials_;
    utl::vector<render::SubMesh> sub_meshes_;
    utl::vector<render::Mesh> meshes_;
};

}
