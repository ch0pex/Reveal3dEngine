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
#include <memory>

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
    Geometry(const wchar_t *path);
    Geometry(primitive type);
    Geometry(std::vector<render::Vertex> && vertices, std::vector<u32> && indices);
//    Geometry(const Geometry &geo);
    INLINE u32 VertexCount() { return mesh_->vertices_.size(); }
    INLINE u32 IndexCount() {return mesh_->indices_.size(); }

    INLINE std::vector<render::SubMesh>&SubMeshes() { return meshes_; }
    INLINE std::vector<render::Vertex>& Vertices() { return mesh_->vertices_; }
    INLINE std::vector<u32>& Indices() { return mesh_->indices_; }
    INLINE render::Vertex* GetVerticesStart() { return mesh_->vertices_.data(); }
    INLINE u32* GetIndicesStart() { return mesh_->indices_.data(); }

    INLINE u32 RenderInfo() { return mesh_->renderInfo; }
    INLINE void SetRenderInfo(u32 index) { mesh_->renderInfo = index; }

    //TODO: DON'T HARDCODE THIS AND SHOW SUB MESHES IN SCENE GRAPH
    INLINE void SetVisibility(bool visibility) { meshes_[0].visible = visibility; }
    INLINE bool IsVisible() { return meshes_[0].visible;  }
    INLINE math::vec4& Color() { return color_;  }

    void AddMesh(const wchar_t *path);
    void AddMesh(primitive type);

    bool OnGPU { false };

//    INLINE u8 IsDirty() { return isDirty_; }
//    INLINE void UpdateDirty() { assert(isDirty_ > 0); --isDirty_; }

private:
//    u8 isDirty_;
    std::vector<render::SubMesh> meshes_;
    std::shared_ptr<render::Mesh> mesh_;
    math::vec4 color_ {1.0f, 1.0f, 1.0f, 1.0f,};
};

}
