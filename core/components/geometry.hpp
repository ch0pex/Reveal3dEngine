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

#include "render/mesh.hpp"
#include "pool.hpp"

#include <queue>
#include <span>
#include <vector>
#include <set>

namespace reveal3d::core {

class GeometryPool;

class Geometry {
public:
    enum Primitive : u8 {
        cube = 0u,
        plane,
        cylinder,
        sphere,
        cone,
        torus,
        custom,

        count
    };

    /************* Geometry Data ****************/
    struct Data {
        u32 Count() { return meshes.size(); }
        render::Material& Material(id_t id) { return materials.at(id::index(id)); }
        render::Mesh& Mesh(id_t id) { return meshes.at(id::index(id)); }
        std::span<render::SubMesh> SubMeshes(id_t id) { return std::span {subMeshes.begin() + id::index(id), subMeshes.begin() + id::index(id) + 1}; }

        utl::vector<render::Material> materials;
        utl::vector<render::SubMesh> subMeshes;
        utl::vector<render::Mesh> meshes;
    };

    using InitInfo = render::Mesh;

    Geometry();
    Geometry(Geometry& other);
    Geometry(Geometry&& other) noexcept;
    explicit Geometry(id_t id);

    Geometry& operator=(const Geometry& other);
    Geometry& operator=(Geometry&& other) noexcept ;

    [[nodiscard]] u32 VertexCount() const;
    [[nodiscard]] u32 IndexCount() const;

    // NOTE: This will need to be changed when Mesh instancing is implemented
    // For now there is no mesh instancing and only one submesh each mesh and geometry
    void AddMesh(render::Mesh& mesh);
    void AddMesh(Primitive type);

    [[nodiscard]] std::span<render::SubMesh> SubMeshes() const;
    [[nodiscard]] std::vector<render::Vertex> &Vertices() const;
    [[nodiscard]] std::vector<u32> &Indices() const;

    // TODO this should be for each mesh
    void SetVisibility(bool visibility);
    [[nodiscard]] bool IsVisible() const;
    const render::Material& Material();

    void SetDiffuseColor(math::vec4 color);
    void SetRoughness(f32 roughness);
    void SetFresnel(math::vec3 fresnel);
    void SetMatTransform(math::mat4 transform);

    [[nodiscard]] INLINE bool IsAlive() const { return id_ != id::invalid; }
    [[nodiscard]] INLINE id_t Id() const { return id_; }

    [[nodiscard]] u8 Dirty() const;
    void UnDirty() const;
    void SetDirty() const;

private:
    [[nodiscard]] static Pool<Geometry>& Pool();

    id_t id_;
};

template<>
Geometry Pool<Geometry>::AddComponent(id_t entityId) {
    const id_t geometryId{ id_factory_.New() };

    data_.meshes.emplace_back();
    data_.subMeshes.emplace_back();
    data_.materials.emplace_back();
    dirties_.emplace_back(4);
    dirtyIds_.insert(geometryId);
    newComponents_.push(entityId);

    Add(id::index(entityId), geometryId);
    return components_.at(id::index(entityId));
}

template<>
Geometry Pool<Geometry>::AddComponent(id_t entityId, Geometry::InitInfo &&initInfo) {
    const id_t geometryId{ id_factory_.New() };

    data_.meshes.push_back(std::move(initInfo));
    data_.materials.emplace_back();
    dirties_.emplace_back(4);
    dirtyIds_.insert(entityId);
    newComponents_.push(entityId);

    Add(id::index(entityId), geometryId);

    data_.subMeshes.emplace_back(render::SubMesh {
            .shader = render::opaque,
            .vertexPos = 0,
            .indexPos = 0,
            .indexCount = components_.at(id::index(entityId)).IndexCount(),
            .visible = true,
    });


    return components_.at(id::index(entityId));
}

template<>
void Pool<Geometry>::RemoveComponent(id_t id) {
    id_t geometry_id { components_.at(id::index(id)).Id() };
    if (id_factory_.IsAlive(id)) {
        data_.materials.unordered_remove(id::index(geometry_id));
        data_.meshes.unordered_remove(id::index(geometry_id));
        data_.subMeshes.unordered_remove(id::index(geometry_id));
        dirties_.at(id::index(id)) = 0;
        if (dirtyIds_.find(geometry_id) != dirtyIds_.end()) {
            dirtyIds_.erase(geometry_id);
        }
        deletedComponents_.push(geometry_id);
        Remove(geometry_id);
    }
}


}
