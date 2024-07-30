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
#include "common/id.hpp"

#include <queue>
#include <span>
#include <vector>
#include <set>

namespace reveal3d::core {

class GeometryPool;

class Geometry {
public:
    enum primitive : u8 {
        cube,
        plane,
        cylinder,
        sphere,
        cone,
        torus,
        custom,

        count
    };

    using PoolType = GeometryPool&;
    using InitInfo = render::Mesh;

    Geometry();
    Geometry(Geometry& other);
    Geometry(Geometry&& other) noexcept;
    explicit Geometry(id_t id);

    Geometry& operator=(const Geometry& other);
    Geometry& operator=(Geometry&& other) noexcept ;

    u32 VertexCount() const;
    u32 IndexCount() const;

    // NOTE: This will need to be changed when Mesh instancing is implemented
    // For now there is no mesh instancing and only one submesh each mesh
    void AddMesh(render::Mesh& mesh);
    void AddMesh(primitive type);

    std::span<render::SubMesh> SubMeshes() const;
    std::vector<render::Vertex> &Vertices() const;
    std::vector<u32> &Indices() const;

    u32 RenderInfo() const;
    void SetRenderInfo(u32 index) const;

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
    [[nodiscard]] static GeometryPool& Pool();

    id_t id_;
};

class GeometryPool {
public:
    Geometry AddComponent();
    Geometry AddComponent(id_t id);
    Geometry AddComponent(id_t id, Geometry::InitInfo&& initInfo);
    void RemoveComponent(id_t id);

    Geometry At(id_t id);
    Geometry PopNewGeometry();
    INLINE std::set<id_t>&  DirtyElements() { return dirtyIds_; }

    std::vector<Geometry>::iterator begin();
    std::vector<Geometry>::iterator end();

private:
    friend class Geometry;
    render::Mesh& Mesh(id_t id);
    std::span<render::SubMesh> SubMeshes(id_t id);
    id_t PopNewGeometry(id_t id);
    render::Material& Material(id_t id);

    /************** Geometry IDs ****************/

    id::Factory id_factory_;
    std::vector<Geometry> geometry_components_;

    /************* Geometry Data ****************/

    std::vector<render::Material> materials_;
    std::vector<render::SubMesh> subMeshes_;
    std::vector<render::Mesh> meshes_;

    // New geometries must be uploaded to GPU
    std::queue<id_t> newGeometries_;

    // Materials must be updated on GPU
    std::set<id_t>                dirtyIds_;
    std::vector<u8>               dirties_;


};

}
