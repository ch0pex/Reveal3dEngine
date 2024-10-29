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

#include "core/scene.hpp"
#include "render/mesh.hpp"

#include <queue>
#include <span>
#include <vector>
#include <set>

namespace reveal3d::core {

class Geometry {
public:
    enum Primitive : u8 {
        Cube = 0U,
        Plane,
        Cylinder,
        Sphere,
        Cone,
        Torus,
        Custom,

        count
    };

    /************* Geometry data ****************/

    struct Data {
        Data(render::Material& mat, render::SubMesh& submesh,  render::Mesh& meshinfo)
                : material(mat), sub_mesh(submesh), mesh(meshinfo) {}
        render::Material& material;
        render::SubMesh&sub_mesh;
        render::Mesh& mesh;
    };

    using pool_type = Pool<geometry::Pool>;

    Geometry() : id_{id::invalid} {}

    Geometry(id_t id) : id_{id} {}

    Geometry(Geometry &other) : id_{other.id()} {}

    Geometry(Geometry &&other) noexcept : id_{other.id()} {}

    [[nodiscard]] bool isAlive() const { return id_ != id::invalid; }

    [[nodiscard]] id_t id() const { return id_; }

    Geometry &operator=(const Geometry &other) {
        id_ = other.id_;
        return *this;
    }
    Geometry &operator=(Geometry &&other) noexcept {
        id_ = other.id_;
        return *this;
    }

    // Note: for now we only will have one mesh and submesh, so we move the vector instead of appending it
    void addMesh(render::Mesh &mesh) {
        render::SubMesh sub_mesh;
        pool.data().mesh(id_) = std::move(mesh);

        sub_mesh.vertex_pos = vertexCount();
        sub_mesh.index_pos = 0;
        sub_mesh.index_count = indexCount();
    }


    [[nodiscard]] u32 vertexCount() const { return pool.data().mesh(id_).vertices.size(); }

    [[nodiscard]] u32 indexCount() const { return pool.data().mesh(id_).indices.size(); }

    [[nodiscard]] std::span<render::SubMesh> subMeshes() const { return pool.data().subMeshes(id_); }

    [[nodiscard]] std::vector<render::Vertex> &vertices() const { return pool.data().mesh(id_).vertices; }

    [[nodiscard]] std::vector<u32> &indices() const { return pool.data().mesh(id_).indices; }

    void visibility(bool visibility) { pool.data().subMeshes(id_)[0].visible = visibility; }

    [[nodiscard]] bool isVisible() const { return pool.data().subMeshes(id_)[0].visible; }

    render::Material &material() { return pool.data().material(id_); }

    void diffuseColor(math::vec4 color) { pool.data().material(id_).base_color = color; setDirty(); }

    void fresnel(math::vec3 fresnel) { pool.data().material(id_).fresnel = fresnel; setDirty(); }

    void materialTransform(math::mat4 transform) { pool.data().material(id_).mat_transform = transform; setDirty(); }

    void roughness(f32 roughness) { pool.data().material(id_).roughness = roughness; setDirty(); }

    u8 dirty() const { return pool.dirties().at(id::index(id_)); }

    void unDirty() const {
        const id_t idx = id::index(id_);
        if (pool.dirties().at(idx) != 0) {
            --pool.dirties().at(idx);
        }
        else {
            pool.dirties().at(idx) = 0;
        }
    }

    void setDirty() const {
        if (dirty() == 3) {
            return;
        }
        if (dirty() == 0) {
            pool.dirtyIds().insert(id_);
        }
        pool.dirties().at(id::index(id_)) = 3;
    }

    Data data() { return { material(), subMeshes()[0], pool.data().mesh(id_) }; }

private:

    static inline auto& pool = core::scene.componentPool<Geometry>();

    id_t id_;
};

}
