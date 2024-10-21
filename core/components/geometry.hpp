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
 * Geometry entity
 */

#pragma once

#include "pool.hpp"
#include "render/mesh.hpp"

#include <queue>
#include <span>
#include <vector>
#include <set>

namespace reveal3d::core {

class Geometry : GpuStored {
public:
    enum Primitive : u8 {
        Cube = 0u,
        Plane,
        Cylinder,
        Sphere,
        Cone,
        Torus,
        Custom,

        count
    };

    /************* Geometry data ****************/
    struct Pool {
        u32 count() { return meshes.size(); }
        render::Material&material(id_t id) { return materials.at(id::index(id)); }
        render::Mesh& mesh(id_t id) { return meshes.at(id::index(id)); }
        std::span<render::SubMesh> subMeshes(id_t id) { return std::span {sub_meshes.begin() + id::index(id), sub_meshes.begin() + id::index(id) + 1}; }

        utl::vector<render::Material> materials;
        utl::vector<render::SubMesh> sub_meshes;
        utl::vector<render::Mesh> meshes;
    };

    struct Data {
        Data(render::Material& mat, render::SubMesh& submesh,  render::Mesh& meshinfo)
                : material(mat), sub_mesh(submesh), mesh(meshinfo) {}
        render::Material& material;
        render::SubMesh&sub_mesh;
        render::Mesh& mesh;
    };

    using InitInfo = render::Mesh;

    Geometry();
    Geometry(Geometry& other);
    Geometry(Geometry&& other) noexcept;
    explicit Geometry(id_t id);

    Geometry& operator=(const Geometry& other);
    Geometry& operator=(Geometry&& other) noexcept ;

    [[nodiscard]] u32 vertexCount() const;
    [[nodiscard]] u32 indexCount() const;

    // NOTE: This will need to be changed when mesh instancing is implemented
    // For now there is no mesh instancing and only one submesh each mesh and geometry
    void AddMesh(render::Mesh& mesh);
    void AddMesh(Primitive type);

    [[nodiscard]] std::span<render::SubMesh> subMeshes() const;
    [[nodiscard]] std::vector<render::Vertex> &vertices() const;
    [[nodiscard]] std::vector<u32> &indices() const;

    [[nodiscard]] bool isVisible() const;
    const render::Material &material();

    void visibility(bool visibility);
    void diffuseColor(math::vec4 color);
    void roughness(f32 roughness);
    void fresnel(math::vec3 fresnel);
    void materialTransform(math::mat4 transform);

    [[nodiscard]] inline bool isAlive() const { return id_ != id::invalid; }
    [[nodiscard]] inline id_t id() const { return id_; }

    [[nodiscard]] u8 dirty() const;
    void unDirty() const;
    void setDirty() const;
    void update();
    Data data();

private:

    id_t id_;
};

template<>
inline Geometry Pool<Geometry>::addComponent(id_t entity_id) {
    const id_t geometry_id{ id_factory_.New(id::index(entity_id)) };

    data_.meshes.emplace_back();
    data_.sub_meshes.emplace_back();
    data_.materials.emplace_back();
    dirties_.emplace_back(4);
    dirty_ids_.insert(geometry_id);
    new_components_.push(entity_id);

    add(id::index(entity_id), geometry_id);
    return components_.at(id::index(entity_id));
}

template<>
inline Geometry Pool<Geometry>::addComponent(id_t entity_id, Geometry::InitInfo &&init_info) {
    const id_t geometry_id{ id_factory_.New(id::index(entity_id)) };

    data_.meshes.push_back(std::move(init_info));
    data_.materials.emplace_back();
    dirties_.emplace_back(4);
    dirty_ids_.insert(geometry_id);
    new_components_.push(entity_id);

    add(id::index(entity_id), geometry_id);

    data_.sub_meshes.emplace_back(render::SubMesh {
            .shader = render::opaque,
            .vertex_pos = 0,
            .index_pos = 0,
            .index_count = components_.at(id::index(entity_id)).indexCount(),
            .visible = true,
    });

    return components_.at(id::index(entity_id));
}

template<>
inline void Pool<Geometry>::removeComponent(id_t id) {
    id_t geometry_id { components_.at(id::index(id)).id() };
    if (id_factory_.isAlive(geometry_id)) {
        data_.materials.unordered_remove(id::index(geometry_id));
        data_.meshes.unordered_remove(id::index(geometry_id));
        data_.sub_meshes.unordered_remove(id::index(geometry_id));
        dirties_.at(id::index(id)) = 0;
        if (dirty_ids_.find(geometry_id) != dirty_ids_.end()) {
            dirty_ids_.erase(geometry_id);
        }
        deleted_components_.push(geometry_id);
        remove(geometry_id);
    }
}


}
