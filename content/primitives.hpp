/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file primitives.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "core/components/geometry.hpp"

namespace reveal3d::content {

template<core::Geometry::Primitive primitive>
core::Geometry::init_info import_primitive();

template<>
inline core::Geometry::init_info import_primitive<core::Geometry::Cube>() {
    render::Mesh mesh;

    std::vector<render::Vertex> v{
            {{-1.0F, -1.0F, -1.0F}, {0.0F, 0.0F, 0.0F, 0.0F}}, // 0
            {{-1.0F, 1.0F, -1.0F}, {0.0F, 1.0F, 0.0F, 0.0F}}, // 1
            {{1.0F, 1.0F, -1.0F}, {1.0F, 1.0F, 0.0F, 0.0F}}, // 2
            {{1.0F, -1.0F, -1.0F}, {1.0F, 0.0F, 0.0F, 0.0F}}, // 3
            {{-1.0F, -1.0F, 1.0F}, {0.0F, 0.0F, 1.0F, 0.0F}}, // 4
            {{-1.0F, 1.0F, 1.0F}, {0.0F, 1.0F, 1.0F, 0.0F}}, // 5
            {{1.0F, 1.0F, 1.0F}, {1.0F, 1.0F, 1.0F, 0.0F}}, // 6
            {{1.0F, -1.0F, 1.0F}, {1.0F, 0.0F, 1.0F, 0.0F}} // 7
    };

    std::vector<u16> ind = {0, 1, 2, 0, 2, 3, 4, 6, 5, 4, 7, 6, 4, 5, 1, 4, 1, 0,
                            3, 2, 6, 3, 6, 7, 1, 5, 6, 1, 6, 2, 4, 0, 3, 4, 3, 7};

    std::ranges::copy(v.begin(), v.end(), std::back_inserter(mesh.vertices));
    std::ranges::copy(ind.begin(), ind.end(), std::back_inserter(mesh.indices));
    return mesh;
}

template<>
inline core::Geometry::init_info import_primitive<core::Geometry::Plane>() {
    render::Mesh mesh;
    std::vector<render::Vertex> v{{{-1.0F, -1.0F, 0.0F}, {0.2F, 0.2F, 0.2F, 0.0F}},
                                  {{-1.0F, 1.0F, 0.0F}, {0.2F, 0.2F, 0.2F, 0.0F}},
                                  {{1.0F, 1.0F, 0.0F}, {0.2F, 0.2F, 0.2F, 0.0F}},
                                  {{1.0F, -1.0F, 0.0F}, {0.0F, 0.0F, 1.0F, 0.0F}}};

    std::vector<u16> ind = {0, 1, 2, 0, 2, 3};

    std::ranges::copy(v.begin(), v.end(), std::back_inserter(mesh.vertices));
    std::ranges::copy(ind.begin(), ind.end(), std::back_inserter(mesh.indices));

    return mesh;
}

} // namespace reveal3d::content
