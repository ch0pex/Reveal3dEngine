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
    Pool().Data().Mesh(id_) = std::move(mesh);

    subMesh.vertexPos = VertexCount();
    subMesh.indexPos = 0;
    subMesh.indexCount = IndexCount();
}


void Geometry::AddMesh(Geometry::Primitive type) {

    render::SubMesh &subMesh = Pool().Data().SubMeshes(id_)[0]; // Only one submesh for now

    subMesh.vertexPos = VertexCount();
    subMesh.indexPos = IndexCount();
    subMesh.indexCount = IndexCount();

    switch (type) {
        case cube:
            content::GetCubeData(Pool().Data().Mesh(id_).vertices, Pool().Data().Mesh(id_).indices);
            break;
        case plane:
            content::GetPlaneData(Pool().Data().Mesh(id_).vertices, Pool().Data().Mesh(id_).indices);
            break;
        case cylinder:
            content::GetCylinderData(Pool().Data().Mesh(id_).vertices, Pool().Data().Mesh(id_).indices);
            break;
        case sphere:
            content::GetSphereData(Pool().Data().Mesh(id_).vertices, Pool().Data().Mesh(id_).indices);
            break;
        case cone:
            content::GetConeData(Pool().Data().Mesh(id_).vertices, Pool().Data().Mesh(id_).indices);
            break;
        case torus:
            content::GetTorusData(Pool().Data().Mesh(id_).vertices, Pool().Data().Mesh(id_).indices);
            break;
    }

    subMesh.indexCount = IndexCount() - subMesh.indexCount;
}

Pool<core::Geometry> &Geometry::Pool() { return core::scene.ComponentPool<Geometry>(); }

u32 Geometry::VertexCount() const { return Pool().Data().Mesh(id_).vertices.size(); }

u32 Geometry::IndexCount() const { return Pool().Data().Mesh(id_).indices.size(); }

std::span<render::SubMesh> Geometry::SubMeshes() const { return Pool().Data().SubMeshes(id_); }

std::vector<render::Vertex> &Geometry::Vertices() const { return Pool().Data().Mesh(id_).vertices; }

std::vector<u32> &Geometry::Indices() const { return Pool().Data().Mesh(id_).indices; }

void Geometry::SetVisibility(bool visibility) { Pool().Data().SubMeshes(id_)[0].visible = visibility; }

bool Geometry::IsVisible() const { return Pool().Data().SubMeshes(id_)[0].visible; }

const render::Material &Geometry::Material() { return Pool().Data().Material(id_); }

void Geometry::SetDiffuseColor(math::vec4 color) {
    Pool().Data().Material(id_).baseColor = color;
    SetDirty();
}

void Geometry::SetFresnel(math::vec3 fresnel) {
    Pool().Data().Material(id_).fresnel = fresnel;
    SetDirty();
}

void Geometry::SetMatTransform(math::mat4 transform) {
    Pool().Data().Material(id_).matTransform = transform;
    SetDirty();
}

void Geometry::SetRoughness(f32 roughness) {
    Pool().Data().Material(id_).roughness = roughness;
    SetDirty();
}

u8 Geometry::Dirty() const { return Pool().Dirties().at(id::index(id_)); }

void Geometry::UnDirty() const {
    const id_t idx = id::index(Pool().GetMappedId(id_));
    if (Pool().Dirties().at(idx) != 0) {
        --Pool().Dirties().at(idx);
    }
    else {
        Pool().Dirties().at(idx) = 0;
    }
}

void Geometry::SetDirty() const {
    const id_t idx = id::index(Pool().GetMappedId(id_));
    if (Dirty() == 3) {
        return;
    }
    if (Dirty() == 0) {
        Pool().DirtyIds().insert(idx);
    }
    Pool().Dirties().at(idx) = 3;
}

void Geometry::Update() {

}


}