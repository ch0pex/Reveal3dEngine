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


void GetCubeData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices) {
     vertices = {
            { {-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f, 0.0f} }, // 0
            { {-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 0.0f} }, // 1
            { {1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 0.0f, 0.0f} }, // 2
            { {1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 0.0f} }, // 3
            { {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 1.0f, 0.0f} }, // 4
            { {-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 1.0f, 0.0f} }, // 5
            { {1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 0.0f} }, // 6
            { {1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 1.0f, 0.0f} }  // 7
    };

    indices = {
            0, 1, 2, 0, 2, 3,
            4, 6, 5, 4, 7, 6,
            4, 5, 1, 4, 1, 0,
            3, 2, 6, 3, 6, 7,
            1, 5, 6, 1, 6, 2,
            4, 0, 3, 4, 3, 7
    };
}

void GetPlaneData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices) {
    vertices = {
            { { -1.0f, -1.0f, 0.0f }, { 0.2f, 0.2f, 0.2f, 0.0f } },
            { { -1.0f, 1.0f, 0.0f },  { 0.2f, 0.2f, 0.2f, 0.0f } },
            { { 1.0f, 1.0f, 0.0f },   { 0.2f, 0.2f, 0.2f, 0.0f } },
            { { 1.0f, -1.0f, 0.0f },  { 0.0f, 0.0f, 1.0f, 0.0f } }
    };

    indices = {
            0, 1, 2,
            0, 2, 3
    };
}

void GetSphereData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices) {

}

void GetCylinderData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices) {

}

void GetConeData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices) {

}

void GetTorusData(std::vector<render::Vertex> &vertices, std::vector<u16> &indices) {

}

}