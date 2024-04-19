/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_render_info.hpp
 * @version 1.0
 * @date 27/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "resources/dx_resources.hpp"

namespace reveal3d::graphics {

struct RenderInfo {
    RenderInfo(dx::BufferInitInfo &vertexInfo, dx::BufferInitInfo &indexInfo);
    D3D_PRIMITIVE_TOPOLOGY topology { D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
    dx::VertexBuffer vertexBuffer;
    dx::IndexBuffer indexBuffer;
//    u32 constantIndex;
};


}
