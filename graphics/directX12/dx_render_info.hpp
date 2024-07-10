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

struct RenderElement {
    RenderElement(dx12::BufferInitInfo &vertexInfo, dx12::BufferInitInfo &indexInfo);
    D3D_PRIMITIVE_TOPOLOGY topology { D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };
    dx12::VertexBuffer vertexBuffer;
    dx12::IndexBuffer indexBuffer;
//    u32 constantIndex;
};


}
