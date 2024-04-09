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
    AddMesh(path);
}

Geometry::Geometry(Geometry::primitive type) {
    AddMesh(type);
}

Geometry::Geometry(std::vector<render::Vertex> &&vertices, std::vector<u16> &&indices)
        : vertices_(vertices),
          indices_(indices)
{

}
void Geometry::AddMesh(const wchar_t *path) {
    render::Mesh mesh;

    mesh.vertexPos = VertexCount();
    mesh.indexPos = IndexCount();
    mesh.indexCount = IndexCount();

    lastIndex_ = content::GetDataFromObj(path, vertices_, indices_, 0);
    assert(lastIndex_ != 0);

    mesh.indexCount = IndexCount() - mesh.indexCount;
    meshes_.push_back(mesh);
}

void Geometry::AddMesh(Geometry::primitive type) {
    render::Mesh mesh;

    mesh.vertexPos = VertexCount();
    mesh.indexPos = IndexCount();
    mesh.indexCount = IndexCount();

    switch (type) {
        case cube:      lastIndex_ = content::GetCubeData(vertices_, indices_, lastIndex_); break;
        case plane:     lastIndex_ = content::GetPlaneData(vertices_, indices_, lastIndex_); break;
        case cylinder:  lastIndex_ = content::GetCylinderData(vertices_, indices_, lastIndex_); break;
        case sphere:    lastIndex_ = content::GetSphereData(vertices_, indices_, lastIndex_); break;
        case cone:      lastIndex_ = content::GetConeData(vertices_, indices_, lastIndex_); break;
        case torus:     lastIndex_ = content::GetTorusData(vertices_, indices_, lastIndex_); break;
    }

    assert(lastIndex_ != 0);

    mesh.indexCount = IndexCount() - mesh.indexCount;
    meshes_.push_back(mesh);

}
Geometry::Geometry(const Geometry &geo) {
    lastIndex_ = geo.lastIndex_;
    vertices_ = geo.vertices_;
    indices_ = geo.indices_;
    for (auto& mesh : geo.meshes_) {
       meshes_.push_back(mesh);
    }
}

}