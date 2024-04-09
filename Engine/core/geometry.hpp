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

#include <vector>

namespace reveal3d::core {

/*TODO: move this to SubMesh and have multiple meshes in an entity geometry? */
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

    Geometry() = default;
//    ~Geometry();
    Geometry(const wchar_t *path);
    Geometry(primitive type);
    Geometry(std::vector<render::Vertex> && vertices, std::vector<u16> && indices);
    Geometry(const Geometry &geo);
    INLINE u32 VertexCount() { return mesh_->vertices_.size(); }
    INLINE u32 IndexCount() {return mesh_->indices_.size(); }

    INLINE render::Vertex* GetVerticesStart() { return mesh_->vertices_.data(); }
    INLINE u16* GetIndicesStart() { return mesh_->indices_.data(); }
    INLINE std::vector<render::SubMesh>& Meshes() { return meshes_; }
    INLINE u32 RenderInfo() { return mesh_->renderInfo; }

    void AddMesh(const wchar_t *path);
    void AddMesh(primitive type);
private:
    std::vector<render::SubMesh> meshes_;
    std::shared_ptr<render::Mesh> mesh_;
};

}
