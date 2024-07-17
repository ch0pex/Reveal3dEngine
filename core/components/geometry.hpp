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
    };

    union InitInfo { 
        primitive primitive;
        render::Mesh* mesh;
    };

    Geometry() = default;
    Geometry(id_t id);
    Geometry(id_t id, InitInfo& initInfo);
    //    Component(const Component &geo);
    u32 VertexCount() { return mesh_->vertices_.size(); }
    u32 IndexCount() { return mesh_->indices_.size(); }

    std::vector<render::SubMesh> &SubMeshes() { return meshes_; }
    std::vector<render::Vertex> &Vertices() { return mesh_->vertices_; }
    std::vector<u32> &Indices() { return mesh_->indices_; }
    render::Vertex *GetVerticesStart() { return mesh_->vertices_.data(); }
    u32 *GetIndicesStart() { return mesh_->indices_.data(); }

    u32 RenderInfo() { return mesh_->renderInfo; }
    void SetRenderInfo(u32 index) { mesh_->renderInfo = index; }

    void SetVisibility(bool visibility) { meshes_[0].visible = visibility; }
    bool IsVisible() { return meshes_[0].visible; }
    math::vec4 &Color() { return color_; }

    void AddMesh(std::shared_ptr<render::Mesh> mesh_);
    void AddMesh(primitive type);

    //    INLINE u8 IsDirty() { return isDirty_; }
    //    INLINE void UpdateDirty() { assert(isDirty_ > 0); --isDirty_; }

private:
    [[nodiscard]] GeometryPool& Pool() const;

    id_t id_;
};

class GeometryPool {
public:
    std::shared_ptr<render::Mesh> Mesh(id_t id);
    std::span<render::Mesh> SubMeshes(id_t id);
    math::vec4 Color(id_t id);
    id_t PopNewGeometry(id_t id);

private:

    /************** Geometry IDs ****************/

    id::Factory id_factory_;
    std::vector<Geometry> geometry_components_;

    /************* Geometry Data ****************/

    std::vector<math::vec4> colors_;
    std::vector<render::SubMesh> subMeshes_;
    std::vector<std::shared_ptr<render::Mesh>> meshes_;

    // New geometries must be uploaded to GPU
    std::queue<id_t> newGeometries_;

};

}
