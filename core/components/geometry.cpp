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

static Pool<core::Geometry>& pool() { return core::scene.componentPool<Geometry>(); }

Geometry::Geometry() : id_{id::invalid} {}

Geometry::Geometry(id_t id) : id_{id} {}

Geometry::Geometry(Geometry &other) : id_{other.id()} {}

Geometry::Geometry(Geometry &&other) noexcept : id_{other.id()} {}

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
void Geometry::addMesh(render::Mesh &mesh) {
    render::SubMesh sub_mesh;
    pool().data().mesh(id_) = std::move(mesh);

    sub_mesh.vertex_pos = vertexCount();
    sub_mesh.index_pos = 0;
    sub_mesh.index_count = indexCount();
}


void Geometry::addMesh(Primitive type) {

    render::SubMesh &sub_mesh = pool().data().subMeshes(id_)[0]; // Only one submesh for now

    sub_mesh.vertex_pos = vertexCount();
    sub_mesh.index_pos = indexCount();
    sub_mesh.index_count = indexCount();

    switch (type) {
        case Cube:
            content::getCubeData(pool().data().mesh(id_).vertices, pool().data().mesh(id_).indices);
            break;
        case Plane:
            content::getPlaneData(pool().data().mesh(id_).vertices, pool().data().mesh(id_).indices);
            break;
        case Cylinder:
            content::getCylinderData(pool().data().mesh(id_).vertices, pool().data().mesh(id_).indices);
            break;
        case Sphere:
            content::getSphereData(pool().data().mesh(id_).vertices, pool().data().mesh(id_).indices);
            break;
        case Cone:
            content::getConeData(pool().data().mesh(id_).vertices, pool().data().mesh(id_).indices);
            break;
        case Torus:
            content::getTorusData(pool().data().mesh(id_).vertices, pool().data().mesh(id_).indices);
            break;
    }

    sub_mesh.index_count = indexCount() - sub_mesh.index_count;
}


u32 Geometry::vertexCount() const { return pool().data().mesh(id_).vertices.size(); }

u32 Geometry::indexCount() const { return pool().data().mesh(id_).indices.size(); }

std::span<render::SubMesh> Geometry::subMeshes() const { return pool().data().subMeshes(id_); }

std::vector<render::Vertex> &Geometry::vertices() const { return pool().data().mesh(id_).vertices; }

std::vector<u32> &Geometry::indices() const { return pool().data().mesh(id_).indices; }

void Geometry::visibility(bool visibility) { pool().data().subMeshes(id_)[0].visible = visibility; }

bool Geometry::isVisible() const { return pool().data().subMeshes(id_)[0].visible; }

render::Material &Geometry::material() { return pool().data().material(id_); }

void Geometry::diffuseColor(math::vec4 color) {
    pool().data().material(id_).base_color = color;
    setDirty();
}

void Geometry::fresnel(math::vec3 fresnel) {
    pool().data().material(id_).fresnel = fresnel;
    setDirty();
}

void Geometry::materialTransform(math::mat4 transform) {
    pool().data().material(id_).mat_transform = transform;
    setDirty();
}

void Geometry::roughness(f32 roughness) {
    pool().data().material(id_).roughness = roughness;
    setDirty();
}

u8 Geometry::dirty() const { return pool().dirties().at(id::index(id_)); }

void Geometry::unDirty() const {
    const id_t idx = id::index(id_);
    if (pool().dirties().at(idx) != 0) {
        --pool().dirties().at(idx);
    }
    else {
        pool().dirties().at(idx) = 0;
    }
}

void Geometry::setDirty() const {
    if (dirty() == 3) {
        return;
    }
    if (dirty() == 0) {
        pool().dirtyIds().insert(id_);
    }
    pool().dirties().at(id::index(id_)) = 3;
}

void Geometry::update() {

}

Geometry::Data Geometry::data() {
    return { material(), subMeshes()[0], pool().data().mesh(id_) };
}

}