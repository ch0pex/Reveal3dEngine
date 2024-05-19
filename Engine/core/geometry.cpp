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
#include "content/obj_parser.hpp"

namespace reveal3d::core {

Geometry::Geometry(const wchar_t *path) {
    mesh_ = std::make_shared<render::Mesh>();
    AddMesh(path);
}

Geometry::Geometry(Geometry::primitive type) {
    mesh_ = std::make_shared<render::Mesh>();
    AddMesh(type);
}

Geometry::Geometry(std::vector<render::Vertex> &&vertices, std::vector<u32> &&indices)
{
    mesh_ = std::make_shared<render::Mesh>(vertices, indices);
}
void Geometry::AddMesh(const wchar_t *path) {
    render::SubMesh mesh;

    mesh.vertexPos = VertexCount();
    mesh.indexPos = IndexCount();
    mesh.indexCount = IndexCount();

    content::GetDataFromObj(path, mesh_->vertices_, mesh_->indices_);

    mesh.indexCount = IndexCount() - mesh.indexCount;
    meshes_.push_back(mesh);
}

void Geometry::AddMesh(Geometry::primitive type) {
    render::SubMesh mesh;

    mesh.vertexPos = VertexCount();
    mesh.indexPos = IndexCount();
    mesh.indexCount = IndexCount();

    switch (type) {
        case cube:       content::GetCubeData(mesh_->vertices_, mesh_->indices_); break;
        case plane:      content::GetPlaneData(mesh_->vertices_, mesh_->indices_); break;
        case cylinder:   content::GetCylinderData(mesh_->vertices_, mesh_->indices_); break;
        case sphere:     content::GetSphereData(mesh_->vertices_, mesh_->indices_); break;
        case cone:       content::GetConeData(mesh_->vertices_, mesh_->indices_); break;
        case torus:      content::GetTorusData(mesh_->vertices_, mesh_->indices_); break;
    }

    mesh.indexCount = IndexCount() - mesh.indexCount;
    meshes_.push_back(mesh);

}
Geometry::Geometry(const Geometry &geo) {
    mesh_ = geo.mesh_;
    meshes_ = geo.meshes_;
}



}