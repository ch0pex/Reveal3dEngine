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

u16 getCubeData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
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


//    ApplyOffset(ind, ++offset);

    vertices.insert(vertices.end(), v.begin(), v.end());
    indices.insert(indices.end(), ind.begin(), ind.end());
    return 7;
}

u16 getPlaneData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
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
    vertices.insert(vertices.end(), v.begin(), v.end());
    indices.insert(indices.end(), ind.begin(), ind.end());
    return 3;
}

u16 getSphereData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
    return 0;
}

u16 getCylinderData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
    return 0;
}

u16 getConeData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
    return 0;
}

u16 getTorusData(std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
    return 0;
}

}