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
    // For now there is no mesh instancing and only one submesh each mesh
    void AddMesh(render::Mesh& mesh);
    void AddMesh(primitive type);

    [[nodiscard]] std::span<render::SubMesh> SubMeshes() const;
    [[nodiscard]] std::vector<render::Vertex> &Vertices() const;
    [[nodiscard]] std::vector<u32> &Indices() const;

    [[nodiscard]] u32 RenderInfo() const;
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

class GeometryPool : public Pool<Geometry>{
public:

    //TODO: This is just a provisional fix, can't store submeshes as pointers in render layers
    GeometryPool() { subMeshes_.reserve(4000);}

    Geometry AddComponent();
    Geometry AddComponent(id_t entityId);
    Geometry AddComponent(id_t entityId, Geometry::InitInfo&& initInfo);
    void RemoveComponent(id_t id) override;
    void Update() override;

    INLINE u32 Count() override { return meshes_.size(); }
    Geometry PopNewGeometry();
    Geometry PopRemovedGeometry();
    INLINE std::set<id_t>&  DirtyElements() { return dirtyIds_; }

private:
    friend class Geometry;
    render::Mesh& Mesh(id_t id);
    std::span<render::SubMesh> SubMeshes(id_t id);
    id_t PopNewGeometry(id_t id);
    render::Material& Material(id_t id);

    /************* Geometry Data ****************/

    utl::vector<render::Material> materials_;
    utl::vector<render::SubMesh> subMeshes_;
    utl::vector<render::Mesh> meshes_;

    // New and removed geometries must be updated on GPU
    std::queue<id_t> newGeometries_;
    std::queue<id_t> delGeometries_;

    // Materials must be updated on GPU

    std::set<id_t>     dirtyIds_;
    std::vector<u8>    dirties_;


};

}
