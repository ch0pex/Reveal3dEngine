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
    switch (type) {
        case cube:      content::GetCubeData(vertices_, indices_); break;
        case plane:     content::GetPlaneData(vertices_, indices_); break;
        case cylinder:  content::GetCylinderData(vertices_, indices_); break;
        case sphere:    content::GetSphereData(vertices_, indices_); break;
        case cone:      content::GetConeData(vertices_, indices_); break;
        case torus:     content::GetTorusData(vertices_, indices_); break;
    }
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

    lastIndex = content::GetDataFromObj(path, vertices_, indices_, lastIndex);
    assert(lastIndex != 0);

    mesh.indexCount = lastIndex - mesh.indexCount;
    meshes_.push_back(mesh);
}

}