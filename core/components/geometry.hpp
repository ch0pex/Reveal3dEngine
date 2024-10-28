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


class Geometry {
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
    using InitInfo = render::Mesh;

    class Pool {
    public:
        render::Material&material(id_t id) { return materials_.at(id::index(id)); }

        render::Mesh& mesh(id_t id) { return meshes_.at(id::index(id)); }

        std::span<render::SubMesh> subMeshes(id_t id) { return std::span {sub_meshes_.begin() + id::index(id), sub_meshes_.begin() + id::index(id) + 1}; }

        u32 count() { return meshes_.size(); }

        void add(id_t entity_id, InitInfo &init_info) {
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

        void remove(u32 id) {
            materials_.unordered_remove(id::index(id));
            meshes_.unordered_remove(id::index(id));
            sub_meshes_.unordered_remove(id::index(id));
        }
    private:
        utl::vector<render::Material> materials_;
        utl::vector<render::SubMesh> sub_meshes_;
        utl::vector<render::Mesh> meshes_;
    };

    struct Data {
        Data(render::Material& mat, render::SubMesh& submesh,  render::Mesh& meshinfo)
                : material(mat), sub_mesh(submesh), mesh(meshinfo) {}
        render::Material& material;
        render::SubMesh&sub_mesh;
        render::Mesh& mesh;
    };


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
    void addMesh(render::Mesh &mesh);
    template<Primitive primitive> void addMesh();

    [[nodiscard]] std::span<render::SubMesh> subMeshes() const;
    [[nodiscard]] std::vector<render::Vertex> &vertices() const;
    [[nodiscard]] std::vector<u32> &indices() const;

    [[nodiscard]] bool isVisible() const;
    render::Material &material();

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
    Data data();

private:
    id_t id_;
};

template<>
inline Geometry Pool<Geometry>::addComponent(id_t entity_id) {
    const id_t geometry_id{ id_factory_.New(id::index(entity_id)) };

    data_.meshes_.emplace_back();
    data_.sub_meshes_.emplace_back();
    data_.materials_.emplace_back();
    dirties_.emplace_back(4);
    dirty_ids_.insert(geometry_id);
    new_components_.push(entity_id);

    data_.add(entity_id, {})
    addId(id::index(entity_id), geometry_id);
    return components_ids_.at(id::index(entity_id));
}

template<>
inline Geometry Pool<Geometry>::addComponent(id_t entity_id, Geometry::InitInfo &&init_info) {
    const id_t geometry_id{ id_factory_.New(id::index(entity_id)) };

    dirties_.emplace_back(4);
    dirty_ids_.insert(geometry_id);
    new_components_.push(entity_id);

    data_.add(entity_id, init_info);
    addId(id::index(entity_id), geometry_id);

    return components_ids_.at(id::index(entity_id));
}

template<>
inline void Pool<Geometry>::removeComponent(id_t id) {
    id_t geometry_id {components_ids_.at(id::index(id)).id() };
    if (id_factory_.isAlive(geometry_id)) {
        components_data_.materials_.unordered_remove(id::index(geometry_id));
        components_data_.meshes_.unordered_remove(id::index(geometry_id));
        components_data_.sub_meshes_.unordered_remove(id::index(geometry_id));
        dirties_.at(id::index(id)) = 0;
        if (dirty_ids_.find(geometry_id) != dirty_ids_.end()) {
            dirty_ids_.erase(geometry_id);
        }
        deleted_components_.push(geometry_id);
        removeId(geometry_id);
    }
}


}
