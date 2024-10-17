/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file geometry.cpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "geometry.hpp"

#include "content/primitives.hpp"
#include "core/scene.hpp"

namespace reveal3d::core {

static Pool<core::Geometry>& pool() { return core::scene.ComponentPool<Geometry>(); }

Geometry::Geometry() : id_{id::invalid} {}

Geometry::Geometry(id_t id) : id_{id} {}

Geometry::Geometry(Geometry &other) : id_{other.Id()} {}

Geometry::Geometry(Geometry &&other) noexcept : id_{other.Id()} {}

Geometry &Geometry::operator=(const Geometry &other) {
    id_ = other.id_;
    return *this;
}
Geometry &Geometry::operator=(Geometry &&other) noexcept {
    id_ = other.id_;
    return *this;
}

// Note: for now we only will have one mesh and submesh so we move the vector
// instead of appending it
void Geometry::AddMesh(render::Mesh &mesh) {
    render::SubMesh subMesh;
    pool().Data().Mesh(id_) = std::move(mesh);

    subMesh.vertexPos = VertexCount();
    subMesh.indexPos = 0;
    subMesh.indexCount = IndexCount();
}


void Geometry::AddMesh(Geometry::Primitive type) {

    render::SubMesh &subMesh = pool().Data().SubMeshes(id_)[0]; // Only one submesh for now

    subMesh.vertexPos = VertexCount();
    subMesh.indexPos = IndexCount();
    subMesh.indexCount = IndexCount();

    switch (type) {
        case cube:
            content::GetCubeData(pool().Data().Mesh(id_).vertices, pool().Data().Mesh(id_).indices);
            break;
        case plane:
            content::GetPlaneData(pool().Data().Mesh(id_).vertices, pool().Data().Mesh(id_).indices);
            break;
        case cylinder:
            content::GetCylinderData(pool().Data().Mesh(id_).vertices, pool().Data().Mesh(id_).indices);
            break;
        case sphere:
            content::GetSphereData(pool().Data().Mesh(id_).vertices, pool().Data().Mesh(id_).indices);
            break;
        case cone:
            content::GetConeData(pool().Data().Mesh(id_).vertices, pool().Data().Mesh(id_).indices);
            break;
        case torus:
            content::GetTorusData(pool().Data().Mesh(id_).vertices, pool().Data().Mesh(id_).indices);
            break;
    }

    subMesh.indexCount = IndexCount() - subMesh.indexCount;
}


u32 Geometry::VertexCount() const { return pool().Data().Mesh(id_).vertices.size(); }

u32 Geometry::IndexCount() const { return pool().Data().Mesh(id_).indices.size(); }

std::span<render::SubMesh> Geometry::SubMeshes() const { return pool().Data().SubMeshes(id_); }

std::vector<render::Vertex> &Geometry::Vertices() const { return pool().Data().Mesh(id_).vertices; }

std::vector<u32> &Geometry::Indices() const { return pool().Data().Mesh(id_).indices; }

void Geometry::SetVisibility(bool visibility) { pool().Data().SubMeshes(id_)[0].visible = visibility; }

bool Geometry::IsVisible() const { return pool().Data().SubMeshes(id_)[0].visible; }

const render::Material &Geometry::Material() { return pool().Data().Material(id_); }

void Geometry::SetDiffuseColor(math::vec4 color) {
    pool().Data().Material(id_).baseColor = color;
    SetDirty();
}

void Geometry::SetFresnel(math::vec3 fresnel) {
    pool().Data().Material(id_).fresnel = fresnel;
    SetDirty();
}

void Geometry::SetMatTransform(math::mat4 transform) {
    pool().Data().Material(id_).matTransform = transform;
    SetDirty();
}

void Geometry::SetRoughness(f32 roughness) {
    pool().Data().Material(id_).roughness = roughness;
    SetDirty();
}

u8 Geometry::Dirty() const { return pool().Dirties().at(id::index(id_)); }

void Geometry::UnDirty() const {
    const id_t idx = id::index(id_);
    if (pool().Dirties().at(idx) != 0) {
        --pool().Dirties().at(idx);
    }
    else {
        pool().Dirties().at(idx) = 0;
    }
}

void Geometry::SetDirty() const {
    if (Dirty() == 3) {
        return;
    }
    if (Dirty() == 0) {
        pool().DirtyIds().insert(id_);
    }
    pool().Dirties().at(id::index(id_)) = 3;
}

void Geometry::Update() {

}


}