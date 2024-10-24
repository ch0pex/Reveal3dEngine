/************************************************************************
 * copyright (c) 2024 alvaro cabrera barrio
 * this code is licensed under mit license (see license.txt for details)
 ************************************************************************/
/**
 * @file primitives.cpp
 * @version 1.0
 * @date 26/03/2024
 * @brief obj and fbx functions
 *
 * methods to parse obj and fbx functions
 *
 */


#include "primitives.hpp"

namespace reveal3d::content {

template<>
core::Geometry::InitInfo import_primitive<core::Geometry::Cube>() {
    render::Mesh mesh;

    std::vector<render::Vertex> v = {
            { {-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f, 0.0f} }, // 0
            { {-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 0.0f} }, // 1
            { {1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 0.0f, 0.0f} }, // 2
            { {1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 0.0f} }, // 3
            { {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 1.0f, 0.0f} }, // 4
            { {-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 1.0f, 0.0f} }, // 5
            { {1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 0.0f} }, // 6
            { {1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 1.0f, 0.0f} }  // 7
    };

    std::vector<u16> ind = {
            0, 1, 2, 0, 2, 3,
            4, 6, 5, 4, 7, 6,
            4, 5, 1, 4, 1, 0,
            3, 2, 6, 3, 6, 7,
            1, 5, 6, 1, 6, 2,
            4, 0, 3, 4, 3, 7
    };

    mesh.vertices.insert(mesh.vertices.end(), v.begin(), v.end());
    mesh.indices.insert(mesh.indices.end(), ind.begin(), ind.end());
    return mesh;
}

template<>
core::Geometry::InitInfo import_primitive<core::Geometry::Plane>() {
    render::Mesh mesh;
    std::vector<render::Vertex> v = {
            { { -1.0f, -1.0f, 0.0f }, { 0.2f, 0.2f, 0.2f, 0.0f } },
            { { -1.0f, 1.0f, 0.0f },  { 0.2f, 0.2f, 0.2f, 0.0f } },
            { { 1.0f, 1.0f, 0.0f },   { 0.2f, 0.2f, 0.2f, 0.0f } },
            { { 1.0f, -1.0f, 0.0f },  { 0.0f, 0.0f, 1.0f, 0.0f } }
    };

    std::vector<u16> ind = {
            0, 1, 2,
            0, 2, 3
    };

//    ApplyOffset(ind, ++offset);
    mesh.vertices.insert(mesh.vertices.end(), v.begin(), v.end());
    mesh.indices.insert(mesh.indices.end(), ind.begin(), ind.end());
    return mesh;
}

u16 get_sphere_data(std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
    return 0;
}

u16 get_cylinder_data(std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
    return 0;
}

u16 get_cone_data(std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
    return 0;
}

u16 get_torus_data(std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
    return 0;
}

}